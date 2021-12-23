#include "bwStyle.h"

#include "bwWidget.h"

namespace bWidgets {

bwWidget::bwWidget(std::optional<unsigned int> width_hint, std::optional<unsigned int> height_hint)
    : width_hint(width_hint.value_or(bwStyle::s_default_widget_size_hint)),
      height_hint(height_hint.value_or(bwStyle::s_default_widget_size_hint))
{
}

auto bwWidget::isCoordinateInside(const bwPoint& point) const -> bool
{
  return rectangle.isCoordinateInside(point.x, point.y);
}

void bwWidget::createState()
{
  state_ = std::make_unique<bwWidgetState>();
}

auto bwWidget::getBaseState() const -> WidgetBaseState
{
  return state_->base;
}

auto bwWidget::setBaseState(WidgetBaseState value) -> bwWidget&
{
  state_->base = value;
  return *this;
}

auto bwWidget::hide(bool _hidden) -> bwWidget&
{
  hidden = _hidden;
  return *this;
}

auto bwWidget::isHidden() -> bool
{
  return hidden;
}

auto bwWidget::getLabel() const -> const std::string*
{
  return nullptr;
}

auto bwWidget::canAlign() const -> bool
{
  return false;
}

/**
 * \brief Additional initialization that can't be done in #bwWidget constructor.
 */
void bwWidget::initialize()
{
  /* Has to be done in init, can't be called from bwWidget constructor (wouldn't call overwritten
   * function). */
  registerProperties();
}

void bwWidget::registerProperties()
{
}

}  // namespace bWidgets
