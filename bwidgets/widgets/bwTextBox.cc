#include "bwStyle.h"

#include "bwTextBox.h"

using namespace bWidgets;


bwTextBox::bwTextBox(
        unsigned int position_x, unsigned int position_y,
        unsigned int width, unsigned int height) :
    bwWidget(WIDGET_TYPE_TEXT_BOX, bwRectanglePixel(position_x, position_x + width, position_y, position_y + height)),
    selection_rectangle(bwRectanglePixel())
{
	
}

void bwTextBox::draw(bwStyle& style) const
{
	bwWidgetStyle& widget_style = style.widget_styles[type];
	bwRectanglePixel inner_rect = rectangle;
	bwPainter painter;

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);

	style.setWidgetStyle(*this);
	painter.setContentMask(inner_rect);

	painter.enableGradient(
	            widget_style.backgroundColor(state),
	            widget_style.shadeTop(state, false), widget_style.shadeBottom(state, false),
	            bwGradient::DIRECTION_TOP_BOTTOM);
	painter.drawRoundbox(inner_rect, widget_style.roundbox_corners, widget_style.roundbox_radius - 1.0f);
	// Outline
	painter.setActiveColor(widget_style.outlineColor(state));
	painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawRoundbox(rectangle, widget_style.roundbox_corners, widget_style.roundbox_radius);

	// Text editing
	if ((state == STATE_SUNKEN) && (selection_rectangle.isEmpty() == false)) {
		// Selection drawing
		painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_FILLED;
		painter.setActiveColor(widget_style.decorationColor(state));
		painter.drawRectangle(selection_rectangle);
	}
	painter.setActiveColor(widget_style.textColor(state));
	painter.drawText(text, rectangle, widget_style.text_alignment, bwPainter::text_draw_arg);
}

void bwTextBox::onClick(const MouseButton mouse_button)
{
	if (mouse_button == MOUSE_BUTTON_LEFT) {
		state = STATE_SUNKEN;
	}
	else if (mouse_button == MOUSE_BUTTON_RIGHT) {
		if (state == STATE_SUNKEN) {
			state = STATE_NORMAL;
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

void bwTextBox::setText(const std::string& value)
{
	text = value;
}

const std::string&bwTextBox::getText() const
{
	return text;
}
