#include "bwRadioButton.h"
#include "bwEvent.h"

namespace bWidgets {

bwRadioButton::bwRadioButton(const std::string& text,
                             std::optional<unsigned int> width_hint,
                             std::optional<unsigned int> height_hint)
    : bwAbstractButton(text, width_hint, height_hint)
{
}

auto bwRadioButton::getTypeIdentifier() const -> std::string_view
{
  return "bwRadioButton";
}

auto bwRadioButton::canAlign() const -> bool
{
  return true;
}

// ------------------ Handling ------------------

class bwRadioButtonHandler : public bwAbstractButtonHandler<bwRadioButton> {
 public:
  bwRadioButtonHandler(bwScreenGraph::Node& node);
  ~bwRadioButtonHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;
};

bwRadioButtonHandler::bwRadioButtonHandler(bwScreenGraph::Node& node)
    : bwAbstractButtonHandler(node)
{
}

auto bwRadioButton::createHandler(bwScreenGraph::Node& node) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwRadioButtonHandler>(node);
}

void bwRadioButtonHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    Widget().setBaseState(bwWidgetState::SUNKEN);
    apply();
    event.swallow();
  }
}

void bwRadioButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  event.swallow();
}

}  // namespace bWidgets
