#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"
#include "screen_graph/Node.h"

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

auto bwAbstractButton::matches(const bwWidget& other) const -> bool
{
  const bwAbstractButton* other_button = widget_cast<bwAbstractButton>(other);

  /* If there's no apply-functor with data to compare, use the label to identify the button. */
  if (!apply_functor && !other_button->apply_functor) {
    return (text == other_button->text);
  }

  return compareFunctors(apply_functor, other_button->apply_functor);
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

/**
 * Default handler for buttons. Needs to be overriden to create custom handlers.
 */
auto bwAbstractButton::createHandler(bwScreenGraph::Node& node) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwAbstractButtonHandler<bwAbstractButton>>(node);
}

// ------------------ Handling ------------------

void bwAbstractButtonHandlerPImpl::onMouseEnter(bwEvent& /*event*/, bwAbstractButton& button)
{
  if (button.getBaseState() == bwWidgetState::NORMAL) {
    button.setBaseState(bwWidgetState::HIGHLIGHTED);
  }
}
void bwAbstractButtonHandlerPImpl::onMouseLeave(bwEvent& /*event*/, bwAbstractButton& button)
{
  if (button.getBaseState() == bwWidgetState::HIGHLIGHTED) {
    button.setBaseState(bwWidgetState::NORMAL);
  }
}

void bwAbstractButtonHandlerPImpl::onMousePress(bwMouseButtonEvent& event,
                                                bwAbstractButton& button)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    button.setBaseState(bwWidgetState::SUNKEN);
    event.swallow();
  }
}
void bwAbstractButtonHandlerPImpl::onMouseRelease(bwMouseButtonEvent& event,
                                                  bwAbstractButton& button)
{
  if ((event.button == bwMouseButtonEvent::Button::LEFT) &&
      (button.getBaseState() == bwWidgetState::SUNKEN)) {
    button.setBaseState(bwWidgetState::NORMAL);

    event.swallow();
  }
}

void bwAbstractButtonHandlerPImpl::apply(bwAbstractButton& button)
{
  if (button.apply_functor) {
    (*button.apply_functor)();
  }
}

}  // namespace bWidgets
