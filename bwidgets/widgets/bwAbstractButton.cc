#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"

#include "bwAbstractButton.h"

namespace bWidgets {

bwAbstractButton::bwAbstractButton(const std::string& text,
                                   std::optional<unsigned int> width_hint,
                                   std::optional<unsigned int> height_hint)
    : bwWidget(width_hint, height_hint),
      rounded_corners(RoundboxCorner::ALL),
      text(std::move(text))
{
  initialize();
}

void bwAbstractButton::draw(bwStyle& style)
{
  const bwGradient gradient{
      base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(base_style, style, rectangle, gradient, base_style.corner_radius);

  // Text
  painter.setContentMask(rectangle);
  painter.setActiveColor(base_style.textColor());
  painter.drawTextAndIcon(text, getIcon(), rectangle, base_style.text_alignment, style.dpi_fac);
}

void bwAbstractButton::registerProperties()
{
  base_style.registerProperties(style_properties);
}

auto bwAbstractButton::getLabel() const -> const std::string*
{
  return &text;
}
auto bwAbstractButton::setLabel(const std::string& label) -> bwAbstractButton&
{
  text = label;
  return *this;
}

auto bwAbstractButton::getIcon() const -> const bwIconInterface*
{
  return nullptr;
}

auto bwAbstractButton::createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwAbstractButtonHandler>(*this);
}

// ------------------ Handling ------------------

bwAbstractButtonHandler::bwAbstractButtonHandler(bwAbstractButton& button) : button(button)
{
}

void bwAbstractButtonHandler::onMouseEnter(bwEvent&)
{
  if (button.getState() == bwWidget::State::NORMAL) {
    button.setState(bwWidget::State::HIGHLIGHTED);
  }
}

void bwAbstractButtonHandler::onMouseLeave(bwEvent&)
{
  if (button.getState() == bwWidget::State::HIGHLIGHTED) {
    button.setState(bwWidget::State::NORMAL);
  }
}

void bwAbstractButtonHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    button.setState(bwWidget::State::SUNKEN);
    event.swallow();
  }
}

void bwAbstractButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  if ((event.button == bwMouseButtonEvent::Button::LEFT) &&
      (button.getState() == bwWidget::State::SUNKEN)) {
    button.setState(bwWidget::State::NORMAL);

    event.swallow();
  }
}

void bwAbstractButtonHandler::apply()
{
  if (button.apply_functor) {
    (*button.apply_functor)();
  }
}

}  // namespace bWidgets
