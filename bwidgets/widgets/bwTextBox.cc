#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"
#include "screen_graph/Node.h"

#include "bwTextBox.h"

namespace bWidgets {

bwTextBox::bwTextBox(std::optional<unsigned int> width_hint,
                     std::optional<unsigned int> height_hint)
    : bwWidget(width_hint, height_hint), selection_rectangle(bwRectanglePixel())
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
  if (is_text_editing && !selection_rectangle.isEmpty()) {
    // Selection drawing
    painter.active_drawtype = bwPainter::DrawType::FILLED;
    painter.setActiveColor(base_style.decorationColor());
    painter.drawRectangle(selection_rectangle);
  }
  painter.setActiveColor(base_style.textColor());
  painter.drawText(text, rectangle, base_style.text_alignment);
}

auto bwTextBox::matches(const bwWidget& other) const -> bool
{
  const bwTextBox* other_textbox = widget_cast<bwTextBox>(other);
  return (text == other_textbox->text);
}

void bwTextBox::copyState(const bwWidget& from)
{
  bwWidget::copyState(from);

  const bwTextBox* other_textbox = widget_cast<bwTextBox>(from);
  if (!other_textbox) {
    return;
  }

  is_text_editing = other_textbox->is_text_editing;
  selection_rectangle = other_textbox->selection_rectangle;
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
  Widget().setState(bwWidget::State::SUNKEN);
  Widget().is_text_editing = true;
}

void bwTextBoxHandler::endTextEditing()
{
  Widget().setState(bwWidget::State::NORMAL);
  Widget().is_text_editing = false;
}

void bwTextBoxHandler::onMouseEnter(bwEvent&)
{
  if (Widget().getState() == bwWidget::State::NORMAL) {
    Widget().setState(bwWidget::State::HIGHLIGHTED);
  }
}

void bwTextBoxHandler::onMouseLeave(bwEvent&)
{
  if (Widget().getState() == bwWidget::State::HIGHLIGHTED) {
    Widget().setState(bwWidget::State::NORMAL);
  }
}
void bwTextBoxHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    startTextEditing();
    event.swallow();
  }
  else if (event.button == bwMouseButtonEvent::Button::RIGHT) {
    if (Widget().getState() == bwWidget::State::SUNKEN) {
      endTextEditing();
      event.swallow();
    }
  }
}

}  // namespace bWidgets
