#include "bwStyle.h"

#include "bwWidget.h"
#include "data_binding/TypeErasedBinding.h"

namespace bWidgets {

bwWidget::bwWidget(std::optional<unsigned int> width_hint, std::optional<unsigned int> height_hint)
    : width_hint(width_hint.value_or(bwStyle::s_default_widget_size_hint)),
      height_hint(height_hint.value_or(bwStyle::s_default_widget_size_hint))
{
}

auto bwWidget::createState() const -> std::unique_ptr<bwWidgetState>
{
  return std::make_unique<bwWidgetState>();
}

auto bwWidget::getBaseState() const -> WidgetBaseState
{
  return getState<bwWidgetState>().base;
}

auto bwWidget::setBaseState(WidgetBaseState value) -> bwWidget&
{
  getState<bwWidgetState>().base = value;
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

auto bwWidget::matches(const bwWidget& /*other*/) const -> std::optional<bool>
{
  return false;
}

auto bwWidget::matchesBindings(const bwWidget& other) const -> std::optional<bool>
{
  std::optional<TypeErasedBinding> a = getBinding();
  std::optional<TypeErasedBinding> b = other.getBinding();
  if (a) {
    return a == b;
  }
  return std::nullopt;
}

auto bwWidget::getBinding() const -> std::optional<TypeErasedBinding>
{
  return std::nullopt;
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
