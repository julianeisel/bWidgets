#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"

#include "bwAbstractButton.h"

namespace bWidgets {

bwAbstractButton::bwAbstractButton(const std::string& text,
                                   const std::string& identifier,
                                   const unsigned int width_hint,
                                   const unsigned int height_hint)
    : bwWidget(identifier, width_hint, height_hint),
      rounded_corners(RoundboxCorner::ALL),
      text(std::move(text))
{
  initialize();
}

void bwAbstractButton::draw(bwStyle& style)
{
  style.setWidgetStyle(*this);

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

const std::string* bwAbstractButton::getLabel() const
{
  return &text;
}

const bwIconInterface* bwAbstractButton::getIcon() const
{
  return nullptr;
}

bwPtr<bwScreenGraph::EventHandler> bwAbstractButton::createHandler()
{
  return bwPtr_new<bwAbstractButtonHandler>(*this);
}

// ------------------ Handling ------------------

bwAbstractButtonHandler::bwAbstractButtonHandler(bwAbstractButton& button) : button(button)
{
}

void bwAbstractButtonHandler::onMouseEnter(bwEvent&)
{
  if (button.state == bwWidget::State::NORMAL) {
    button.state = bwWidget::State::HIGHLIGHTED;
  }
}

void bwAbstractButtonHandler::onMouseLeave(bwEvent&)
{
  if (button.state == bwWidget::State::HIGHLIGHTED) {
    button.state = bwWidget::State::NORMAL;
  }
}

void bwAbstractButtonHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::BUTTON_LEFT) {
    button.state = bwWidget::State::SUNKEN;
    event.swallow();
  }
}

void bwAbstractButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  if ((event.button == bwMouseButtonEvent::BUTTON_LEFT) &&
      (button.state == bwWidget::State::SUNKEN)) {
    button.state = bwWidget::State::NORMAL;

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
