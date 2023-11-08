#include "bwStyle.h"

#include "bwWidget.h"

namespace bWidgets {

bwWidget::bwWidget(std::optional<unsigned int> width_hint, std::optional<unsigned int> height_hint)
    : state(State::NORMAL),
      rectangle(0, 0, 0, 0),
      width_hint(width_hint.value_or(bwStyle::s_default_widget_size_hint)),
      height_hint(height_hint.value_or(bwStyle::s_default_widget_size_hint))
{
}

auto bwWidget::getState() const -> State
{
  return state;
}

auto bwWidget::setState(State value) -> bwWidget&
{
  state = value;
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
