#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"

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

std::unique_ptr<bwScreenGraph::EventHandler> bwTextBox::createHandler()
{
  return std::make_unique<bwTextBoxHandler>(*this);
}

bwTextBoxHandler::bwTextBoxHandler(bwTextBox& textbox) : textbox(textbox)
{
}

void bwTextBoxHandler::startTextEditing()
{
  textbox.setState(bwWidget::State::SUNKEN);
  textbox.is_text_editing = true;
}

void bwTextBoxHandler::endTextEditing()
{
  textbox.setState(bwWidget::State::NORMAL);
  textbox.is_text_editing = false;
}

void bwTextBoxHandler::onMouseEnter(bwEvent&)
{
  if (textbox.getState() == bwWidget::State::NORMAL) {
    textbox.setState(bwWidget::State::HIGHLIGHTED);
  }
}

void bwTextBoxHandler::onMouseLeave(bwEvent&)
{
  if (textbox.getState() == bwWidget::State::HIGHLIGHTED) {
    textbox.setState(bwWidget::State::NORMAL);
  }
}
void bwTextBoxHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    startTextEditing();
    event.swallow();
  }
  else if (event.button == bwMouseButtonEvent::Button::RIGHT) {
    if (textbox.getState() == bwWidget::State::SUNKEN) {
      endTextEditing();
      event.swallow();
    }
  }
}

}  // namespace bWidgets
