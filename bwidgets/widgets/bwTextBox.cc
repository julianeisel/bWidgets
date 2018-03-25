#include "bwPainter.h"
#include "bwStyle.h"

#include "bwTextBox.h"

using namespace bWidgets;


bwTextBox::bwTextBox(
        unsigned int width_hint, unsigned int height_hint) :
    bwWidget(WIDGET_TYPE_TEXT_BOX, "bwTextBox", width_hint, height_hint),
    selection_rectangle(bwRectanglePixel()), is_text_editing(false)
{
	initialize();
}

void bwTextBox::draw(bwStyle& style)
{
	style.setWidgetStyle(*this);

	bwRectanglePixel inner_rect = rectangle;
	const bwGradient gradient{
	        base_style.backgroundColor(),
	        base_style.shadeTop(), base_style.shadeBottom()
	};
	bwPainter painter;

	painter.drawRoundboxWidgetBase(base_style, style, inner_rect, gradient, base_style.corner_radius);

	// Text editing
	if (is_text_editing && (selection_rectangle.isEmpty() == false)) {
		// Selection drawing
		painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_FILLED;
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

void bwTextBox::mousePressEvent(
        const MouseButton button,
        const bwPoint& /*location*/)
{
	if (button == MOUSE_BUTTON_LEFT) {
		startTextEditing();
	}
	else if (button == MOUSE_BUTTON_RIGHT) {
		if (state == STATE_SUNKEN) {
			endTextEditing();
		}
	}
}

void bwTextBox::mouseEnter()
{
	if (state == STATE_NORMAL) {
		state = STATE_HIGHLIGHTED;
	}
}

void bwTextBox::mouseLeave()
{
	if (state == STATE_HIGHLIGHTED) {
		state = STATE_NORMAL;
	}
}

void bwTextBox::startTextEditing()
{
	state = STATE_SUNKEN;
	is_text_editing = true;
}

void bwTextBox::endTextEditing()
{
	state = STATE_NORMAL;
	is_text_editing = false;
}

void bwTextBox::setText(const std::string& value)
{
	text = value;
}

const std::string&bwTextBox::getText() const
{
	return text;
}
