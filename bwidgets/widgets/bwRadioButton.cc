#include "bwEvent.h"
#include "bwRadioButton.h"

using namespace bWidgets;

bwRadioButton::bwRadioButton(const std::string& text,
                             unsigned int width_hint,
                             unsigned int height_hint)
    : bwAbstractButton(text, Type::RADIO_BUTTON, "bwRadioButton", width_hint, height_hint)
{
}

bool bwRadioButton::canAlign() const
{
  return true;
}

// ------------------ Handling ------------------

class bwRadioButtonHandler : public bwAbstractButtonHandler {
 public:
  bwRadioButtonHandler(bwRadioButton& button);
  ~bwRadioButtonHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;
};

bwRadioButtonHandler::bwRadioButtonHandler(bwRadioButton& button) : bwAbstractButtonHandler(button)
{
}

bwPtr<bwScreenGraph::EventHandler> bwRadioButton::createHandler()
{
  return bwPtr_new<bwRadioButtonHandler>(*this);
}

void bwRadioButtonHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::BUTTON_LEFT) {
    button.state = bwWidget::State::SUNKEN;
    apply();
    event.swallow();
  }
}

void bwRadioButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  event.swallow();
}
