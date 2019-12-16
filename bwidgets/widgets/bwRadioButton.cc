#include "bwRadioButton.h"

using namespace bWidgets;

bwRadioButton::bwRadioButton(const std::string& text,
                             unsigned int width_hint,
                             unsigned int height_hint)
    : bwAbstractButton(text, Type::RADIO_BUTTON, "bwRadioButton", width_hint, height_hint)
{
}

void bwRadioButton::mousePressEvent(const bwWidget::MouseButton button,
                                    const bwPoint& /*location*/)
{
  if (button == bwWidget::MOUSE_BUTTON_LEFT) {
    state = State::SUNKEN;
    apply();
  }
}

void bwRadioButton::mouseReleaseEvent(const bwWidget::MouseButton /*button*/,
                                      const bwPoint& /*location*/)
{
}

bool bwRadioButton::canAlign() const
{
  return true;
}
