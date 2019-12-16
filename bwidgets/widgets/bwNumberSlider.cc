#include <algorithm>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "bwPainter.h"
#include "bwStyle.h"

#include "bwNumberSlider.h"

using namespace bWidgets;

bwNumberSlider::bwNumberSlider(const unsigned int width_hint, const unsigned int height_hint)
    : bwTextBox(width_hint, height_hint), precision(2)
{
  type = Type::NUMBER_SLIDER;
  identifier = "bwNumberSlider";
}

void bwNumberSlider::draw(bwStyle& style)
{
  bwPainter painter;

  style.setWidgetStyle(*this);

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
  painter.drawText(valueToString(precision),
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

void bwNumberSlider::mousePressEvent(const MouseButton button, const bwPoint& /*location*/)
{
  if (button == MOUSE_BUTTON_LEFT) {
    initial_value = value;
  }
  else if (button == MOUSE_BUTTON_RIGHT) {
    if (is_text_editing) {
      endTextEditing();
    }
    else if (is_dragging) {
      value = initial_value;
    }
  }
  is_dragging = true;
}

void bwNumberSlider::mouseReleaseEvent(const MouseButton /*button*/, const bwPoint& /*location*/)
{
  is_dragging = false;
}

void bwNumberSlider::mouseClickEvent(const MouseButton button, const bwPoint& /*location*/)
{
  if (button == MOUSE_BUTTON_LEFT) {
    startTextEditing();
  }
}

void bwNumberSlider::mouseDragEvent(const MouseButton button, const bwDistance drag_distance)
{
  if (button == MOUSE_BUTTON_LEFT) {
    setValue(initial_value + (drag_distance.x / (float)rectangle.width()));
    if (apply_functor) {
      (*apply_functor)();
    }
  }
}

void bwNumberSlider::setValue(float _value)
{
  const int precision_fac = std::pow(10, precision);
  const float unclamped_value = std::max(min, std::min(max, _value));

  value = std::roundf(unclamped_value * precision_fac) / precision_fac;
}

float bwNumberSlider::getValue() const
{
  return value;
}

void bwNumberSlider::setMinMax(float _min, float _max)
{
  min = _min;
  max = _max;
}

std::string bwNumberSlider::valueToString(unsigned int precision) const
{
  std::stringstream string_stream;
  string_stream << std::fixed << std::setprecision(precision) << value;
  return string_stream.str();
}

float bwNumberSlider::calcValueIndicatorWidth(bwStyle& style) const
{
  const float range = max - min;
  const float radius = base_style.corner_radius * style.dpi_fac;

  assert(max > min);
  return ((value - min) * (rectangle.width() - radius)) / range;
}
