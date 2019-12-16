#include "bwEvent.h"
#include "bwRadioButton.h"

using namespace bWidgets;

bwRadioButton::bwRadioButton(const std::string& text,
                             unsigned int width_hint,
                             unsigned int height_hint)
    : bwAbstractButton(text, Type::RADIO_BUTTON, "bwRadioButton", width_hint, height_hint)
{
}

void bwRadioButton::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::BUTTON_LEFT) {
    state = State::SUNKEN;
    apply();
  }
}

void bwRadioButton::onMouseRelease(bwMouseButtonEvent&)
{
}

bool bwRadioButton::canAlign() const
{
  return true;
}
