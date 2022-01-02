#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"
#include "data_binding/TypeErasedBinding.h"
#include "screen_graph/Node.h"

#include "bwCheckbox.h"

namespace bWidgets {

bwCheckbox::bwCheckbox(Binding<bool> binding,
                       const std::string& text,
                       std::optional<unsigned int> width_hint,
                       std::optional<unsigned int> height_hint)
    : bwAbstractButton(text,
                       width_hint.value_or(bwStyle::s_default_widget_size_hint),
                       height_hint.value_or(bwStyle::s_default_widget_size_hint)),
      value_binding_(std::move(binding))
{
  setChecked(value_binding_.get());
}

auto bwCheckbox::getTypeIdentifier() const -> std::string_view
{
  return "bwCheckbox";
}

void bwCheckbox::draw(bwStyle& style)
{
  const bwRectanglePixel checkbox_rect = getCheckboxRectangle();
  const bwRectanglePixel text_rect = getTextRectangle(checkbox_rect);
  const bwGradient gradient{
      base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(
      base_style, style, checkbox_rect, gradient, base_style.corner_radius);

  if (isChecked()) {
    painter.active_drawtype = bwPainter::DrawType::OUTLINE;
    painter.setActiveColor(base_style.decorationColor());
    painter.drawCheckMark(checkbox_rect);
  }

  painter.setContentMask(text_rect);  // Not sure if we should set this here.
  painter.setActiveColor(base_style.textColor());
  painter.drawText(text, text_rect, base_style.text_alignment);
}

auto bwCheckbox::setChecked(bool checked) -> bwCheckbox&
{
  setBaseState(checked ? bwWidgetState::SUNKEN : bwWidgetState::NORMAL);
  value_binding_.set(checked);
  return *this;
}

auto bwCheckbox::isChecked() const -> bool
{
  return value_binding_.get();
}

auto bwCheckbox::getCheckboxRectangle() const -> bwRectanglePixel
{
  bwRectanglePixel checkbox_rect{rectangle};
  const int delta = 1 + checkbox_rect.height() / 8;

  checkbox_rect.xmax = checkbox_rect.xmin + checkbox_rect.height();
  checkbox_rect.resize(-delta);

  return checkbox_rect;
}

auto bwCheckbox::getTextRectangle(const bwRectanglePixel& checkbox_rectangle) const
    -> bwRectanglePixel
{
  bwRectanglePixel text_rect{rectangle};
  text_rect.xmin = checkbox_rectangle.xmax - 1;  // XXX -1 is ugly. Specifically for demo app.
  return text_rect;
}

auto bwCheckbox::matches(const bwWidget&) const -> std::optional<bool>
{
  /* TODO get rid of apply functor. */
  return std::nullopt;
}

auto bwCheckbox::getBinding() const -> std::optional<TypeErasedBinding>
{
  return value_binding_;
}

// ------------------ Handling ------------------

class bwCheckboxHandler : public bwAbstractButtonHandler<bwCheckbox> {
 public:
  bwCheckboxHandler(bwScreenGraph::Node& node);
  ~bwCheckboxHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;
};

auto bwCheckbox::createHandler(bwScreenGraph::Node& node) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwCheckboxHandler>(node);
}

bwCheckboxHandler::bwCheckboxHandler(bwScreenGraph::Node& node) : bwAbstractButtonHandler(node)
{
}

void bwCheckboxHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    bwCheckbox& checkbox = Widget();

    checkbox.setChecked(!checkbox.isChecked());
    if (!checkbox.isChecked()) {
      checkbox.setBaseState(bwWidgetState::HIGHLIGHTED);
    }
    apply();
    event.swallow();
  }
}

void bwCheckboxHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  event.swallow();

  // Don't change state at all.
}

}  // namespace bWidgets
