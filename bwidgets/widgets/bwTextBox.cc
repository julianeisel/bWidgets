#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"
#include "screen_graph/Node.h"

#include "bwTextBox.h"

namespace bWidgets {

bwTextBox::bwTextBox(std::optional<unsigned int> width_hint,
                     std::optional<unsigned int> height_hint)
    : bwWidget(width_hint, height_hint)
{
  initialize();
}

auto bwTextBox::getTypeIdentifier() const -> std::string_view
{
  return "bwTextBox";
}

void bwTextBox::draw(bwStyle& style)
{
  bwRectanglePixel inner_rect = rectangle;
  const bwGradient gradient{
      base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(
      base_style, style, inner_rect, gradient, base_style.corner_radius);

  // Text editing
  if (isTextEditing() && !getSelectionRectangle().isEmpty()) {
    // Selection drawing
    painter.active_drawtype = bwPainter::DrawType::FILLED;
    painter.setActiveColor(base_style.decorationColor());
    painter.drawRectangle(getSelectionRectangle());
  }
  painter.setActiveColor(base_style.textColor());
  painter.drawText(text, rectangle, base_style.text_alignment);
}

auto bwTextBox::matches(const bwWidget& other) const -> bool
{
  const bwTextBox* other_textbox = widget_cast<bwTextBox>(other);
  return (text == other_textbox->text);
}

void bwTextBox::registerProperties()
{
  base_style.registerProperties(style_properties);
}

auto bwTextBox::setText(const std::string& value) -> bwTextBox&
{
  text = value;
  return *this;
}

auto bwTextBox::getLabel() const -> const std::string*
{
  return &text;
}

bool bwTextBox::canAlign() const
{
  return true;
}

// ------------------ State ------------------

void bwTextBox::createState()
{
  state_ = std::make_unique<bwTextBoxState>();
}

auto bwTextBox::state() const -> bwTextBoxState&
{
  return getState<bwTextBoxState>();
}

auto bwTextBox::isTextEditing() const -> bool
{
  return state().is_text_editing;
}

auto bwTextBox::getSelectionRectangle() const -> bwRectanglePixel&
{
  return state().selection_rectangle;
}

// ------------------ Handling ------------------

std::unique_ptr<bwScreenGraph::EventHandler> bwTextBox::createHandler(
    bwScreenGraph::Node& node) const
{
  return std::make_unique<bwTextBoxHandler>(node);
}

bwTextBoxHandler::bwTextBoxHandler(bwScreenGraph::Node& node)
    : bwScreenGraph::WidgetEventHandler<bwTextBox>(node)
{
}

void bwTextBoxHandler::startTextEditing()
{
  Widget().setBaseState(bwWidgetState::SUNKEN);
  Widget().getState<bwTextBoxState>().is_text_editing = true;
}

void bwTextBoxHandler::endTextEditing()
{
  Widget().setBaseState(bwWidgetState::NORMAL);
  Widget().getState<bwTextBoxState>().is_text_editing = false;
}

void bwTextBoxHandler::onMouseEnter(bwEvent&)
{
  if (Widget().getBaseState() == bwWidgetState::NORMAL) {
    Widget().setBaseState(bwWidgetState::HIGHLIGHTED);
  }
}

void bwTextBoxHandler::onMouseLeave(bwEvent&)
{
  if (Widget().getBaseState() == bwWidgetState::HIGHLIGHTED) {
    Widget().setBaseState(bwWidgetState::NORMAL);
  }
}
void bwTextBoxHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    startTextEditing();
    event.swallow();
  }
  else if (event.button == bwMouseButtonEvent::Button::RIGHT) {
    if (Widget().getBaseState() == bwWidgetState::SUNKEN) {
      endTextEditing();
      event.swallow();
    }
  }
}

}  // namespace bWidgets
