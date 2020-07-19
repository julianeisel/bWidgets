#include "bwRadioButton.h"
#include "bwEvent.h"

namespace bWidgets {

bwRadioButton::bwRadioButton(const std::string& text,
                             std::optional<unsigned int> width_hint,
                             std::optional<unsigned int> height_hint)
    : bwAbstractButton(text, "bwRadioButton", width_hint, height_hint)
{
}

auto bwRadioButton::canAlign() const -> bool
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

auto bwRadioButton::createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwRadioButtonHandler>(*this);
}

void bwRadioButtonHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    button.setState(bwWidget::State::SUNKEN);
    apply();
    event.swallow();
  }
}

void bwRadioButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  event.swallow();
}

}  // namespace bWidgets
