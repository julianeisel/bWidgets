#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"
#include "screen_graph/Node.h"

#include "bwNumberSlider.h"

namespace bWidgets {

bwNumberSlider::bwNumberSlider(std::optional<unsigned int> width_hint,
                               std::optional<unsigned int> height_hint)
    : bwTextBox(width_hint, height_hint)
{
}

auto bwNumberSlider::getTypeIdentifier() const -> std::string_view
{
  return "bwNumberSlider";
}

void bwNumberSlider::draw(bwStyle& style)
{
  bwPainter painter;
  bwRectanglePixel inner_rect = rectangle;
  const float radius = base_style.corner_radius * style.dpi_fac;

  // Inner - "inside" of outline, so scale down
  inner_rect.resize(-1);

  painter.setContentMask(inner_rect);

  painter.enableGradient(
      bwGradient(base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()));
  painter.drawRoundbox(inner_rect, base_style.roundbox_corners, radius - 1.0f);

  painter.active_drawtype = bwPainter::DrawType::FILLED;

  // Text editing
  if (is_text_editing) {
    // Selection drawing
    painter.setActiveColor(base_style.decorationColor());
    painter.drawRectangle(selection_rectangle);
  }
  else {
    drawValueIndicator(painter, style);
  }

  // Outline
  painter.setActiveColor(base_style.borderColor());
  painter.active_drawtype = bwPainter::DrawType::OUTLINE;
  painter.drawRoundbox(rectangle, base_style.roundbox_corners, radius);

  painter.setActiveColor(base_style.textColor());
  if (!is_text_editing) {
    painter.drawText(text, rectangle, base_style.text_alignment);
  }
  painter.drawText(valueToString(value_info.precision),
                   rectangle,
                   is_text_editing ? TextAlignment::LEFT : TextAlignment::RIGHT);
}

void bwNumberSlider::drawValueIndicator(bwPainter& painter, bwStyle& style) const
{
  bwGradient gradient = bwGradient(base_style.decorationColor(),
                                   // shadeTop/Bottom intentionally inverted
                                   base_style.shadeBottom(),
                                   base_style.shadeTop());
  bwRectanglePixel indicator_offset_rect = rectangle;
  bwRectanglePixel indicator_rect = rectangle;
  unsigned int roundbox_corners = base_style.roundbox_corners;
  const float radius = base_style.corner_radius * style.dpi_fac;
  float right_side_radius = radius;

  indicator_offset_rect.xmax = indicator_offset_rect.xmin + right_side_radius;

  indicator_rect.xmin = indicator_offset_rect.xmax;
  indicator_rect.xmax = indicator_rect.xmin + calcValueIndicatorWidth(style);
  if (indicator_rect.xmax > (rectangle.xmax - right_side_radius)) {
    right_side_radius *= (indicator_rect.xmax + right_side_radius - rectangle.xmax) /
                         right_side_radius;
  }
  else {
    roundbox_corners &= ~(TOP_RIGHT | BOTTOM_RIGHT);
  }

  painter.enableGradient(gradient);
  painter.drawRoundbox(
      indicator_offset_rect, roundbox_corners & ~(TOP_RIGHT | BOTTOM_RIGHT), radius);
  painter.drawRoundbox(
      indicator_rect, roundbox_corners & ~(TOP_LEFT | BOTTOM_LEFT), right_side_radius);
}

auto bwNumberSlider::setValue(float _value) -> bwNumberSlider&
{
  const int precision_fac = std::pow(10, value_info.precision);
  const float unclamped_value = std::max(value_info.min, std::min(value_info.max, _value));

  value_info.value = std::roundf(unclamped_value * precision_fac) / precision_fac;
  return *this;
}

auto bwNumberSlider::getValue() const -> float
{
  return value_info.value;
}

auto bwNumberSlider::setMinMax(float _min, float _max) -> bwNumberSlider&
{
  value_info.min = _min;
  value_info.max = _max;
  return *this;
}

auto bwNumberSlider::valueToString(unsigned int precision) const -> std::string
{
  std::stringstream string_stream;
  string_stream << std::fixed << std::setprecision(precision) << value_info.value;
  return string_stream.str();
}

auto bwNumberSlider::calcValueIndicatorWidth(bwStyle& style) const -> float
{
  const float range = value_info.max - value_info.min;
  const float radius = base_style.corner_radius * style.dpi_fac;

  assert(value_info.max > value_info.min);
  return ((value_info.value - value_info.min) * (rectangle.width() - radius)) / range;
}

auto bwNumberSlider::matches(const bwWidget& other) const -> bool
{
  const bwNumberSlider* other_slider = widget_cast<bwNumberSlider>(other);
  return bwTextBox::matches(other) && compareFunctors(apply_functor, other_slider->apply_functor);
}

void bwNumberSlider::copyState(const bwWidget& from)
{
  bwWidget::copyState(from);

  const bwNumberSlider* other_slider = widget_cast<bwNumberSlider>(from);
  if (!other_slider) {
    return;
  }

  value_info = other_slider->value_info;
}

// ------------------ Handling ------------------

class bwNumberSliderHandler : public bwTextBoxHandler {
 public:
  bwNumberSliderHandler(bwScreenGraph::Node& node);
  ~bwNumberSliderHandler() = default;

  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;
  void onMouseClick(bwMouseButtonEvent&) override;
  void onMouseDrag(bwMouseButtonDragEvent&) override;

  bwNumberSlider& Widget() const override;

 private:
  // Initial value before starting to edit.
  float initial_value;
};

bwNumberSliderHandler::bwNumberSliderHandler(bwScreenGraph::Node& node) : bwTextBoxHandler(node)
{
}

auto bwNumberSlider::createHandler(bwScreenGraph::Node& node) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwNumberSliderHandler>(node);
}

bwNumberSlider& bwNumberSliderHandler::Widget() const
{
  return *widget_cast<bwNumberSlider>(bwTextBoxHandler::Widget());
}

void bwNumberSliderHandler::onMouseEnter(bwEvent& event)
{
  bwTextBoxHandler::onMouseEnter(event);
}

void bwNumberSliderHandler::onMouseLeave(bwEvent& event)
{
  bwTextBoxHandler::onMouseLeave(event);
}

void bwNumberSliderHandler::onMousePress(bwMouseButtonEvent& event)
{
  bwNumberSlider& numberslider = Widget();

  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    initial_value = numberslider.value_info.value;
    numberslider.setState(bwWidget::State::SUNKEN);

    event.swallow();
  }
  else if (event.button == bwMouseButtonEvent::Button::RIGHT) {
    if (numberslider.is_text_editing) {
      endTextEditing();
    }
    else if (is_dragging) {
      numberslider.value_info.value = initial_value;
    }

    event.swallow();
  }
}

void bwNumberSliderHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  if (is_dragging) {
    Widget().setState(bwWidget::State::NORMAL);
  }
  is_dragging = false;

  event.swallow();
}

void bwNumberSliderHandler::onMouseClick(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    startTextEditing();
  }

  event.swallow();
}

void bwNumberSliderHandler::onMouseDrag(bwMouseButtonDragEvent& event)
{
  bwNumberSlider& numberslider = *widget_cast<bwNumberSlider>(Widget());

  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    numberslider.setValue(initial_value +
                          (event.drag_distance.x / (float)numberslider.rectangle.width()));
    if (numberslider.apply_functor) {
      (*numberslider.apply_functor)();
    }

    is_dragging = true;

    event.swallow();
  }
}

}  // namespace bWidgets
