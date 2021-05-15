#include "bwPushButton.h"

namespace bWidgets {

bwPushButton::bwPushButton(const std::string& text,
                           std::optional<unsigned int> width_hint,
                           std::optional<unsigned int> height_hint)
    : bwAbstractButton(text, "bwPushButton", width_hint, height_hint)
{
}

auto bwPushButton::operator==(const bwWidget& other) const -> bool
{
  const bwPushButton* other_button = widget_cast<bwPushButton>(other);
  if (!other_button) {
    return false;
  }

  return bwAbstractButton::operator==(other);
}

auto bwPushButton::getIcon() const -> const bwIconInterface*
{
  return icon;
}

auto bwPushButton::setIcon(const bwIconInterface& icon_interface) -> bwPushButton&
{
  icon = &icon_interface;
  return *this;
}

auto bwPushButton::canAlign() const -> bool
{
  return true;
}

}  // namespace bWidgets
