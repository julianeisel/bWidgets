#include "bwPushButton.h"

using namespace bWidgets;

bwPushButton::bwPushButton(const std::string& text,
                           unsigned int width_hint,
                           unsigned int height_hint)
    : bwAbstractButton(text, Type::PUSH_BUTTON, "bwPushButton", width_hint, height_hint)
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
