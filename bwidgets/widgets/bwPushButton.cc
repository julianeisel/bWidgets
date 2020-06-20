#include "bwPushButton.h"

namespace bWidgets {

bwPushButton::bwPushButton(const std::string& text,
                           std::optional<unsigned int> width_hint,
                           std::optional<unsigned int> height_hint)
    : bwAbstractButton(text, "bwPushButton", width_hint, height_hint)
{
}

const bwIconInterface* bwPushButton::getIcon() const
{
  return icon;
}

bwPushButton& bwPushButton::setIcon(const bwIconInterface& icon_interface)
{
  icon = &icon_interface;
  return *this;
}

bool bwPushButton::canAlign() const
{
  return true;
}

}  // namespace bWidgets
