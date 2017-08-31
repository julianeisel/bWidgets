#include <cassert>
#include <iomanip>
#include <sstream>

#include "bwStyle.h"

#include "bwNumberSlider.h"

using namespace bWidgets;


bwNumberSlider::bwNumberSlider(
        unsigned int position_x, unsigned int position_y,
        unsigned int width, unsigned int height) :
    bwTextBox(position_x, position_y, width, height)
{
	type = WIDGET_TYPE_NUMBER_SLIDER;
}

void bwNumberSlider::draw(bwStyle& style) const
{
	bwWidgetStyle& widget_style = style.widget_styles[type];
	bwRectanglePixel inner_rect = rectangle;
	bwPainter painter;

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);

	style.setWidgetStyle(*this);

	painter.setContentMask(inner_rect); // Not sure if we should set this here.

	painter.enableGradient(
	            widget_style.backgroundColor(state),
	            widget_style.shadeTop(state), widget_style.shadeBottom(state),
	            bwGradient::DIRECTION_TOP_BOTTOM);
	painter.drawRoundbox(inner_rect, widget_style.roundbox_corners, widget_style.roundbox_radius - 1.0f);

	painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_FILLED;

	// Text editing
	if (is_text_editing) {
		// Selection drawing
		painter.setActiveColor(widget_style.decorationColor(state));
		painter.drawRectangle(selection_rectangle);
	}
	else {
		// Value indicator
		bwRectanglePixel indicator_offset_rect = rectangle;
		bwRectanglePixel indicator_rect = rectangle;
		unsigned int roundbox_corners = widget_style.roundbox_corners;
		float right_side_radius = widget_style.roundbox_radius;

		indicator_offset_rect.xmax = indicator_offset_rect.xmin + right_side_radius;

		indicator_rect.xmin = indicator_offset_rect.xmax;
		indicator_rect.xmax = indicator_rect.xmin + calcValueIndicatorWidth(style);
		if (indicator_rect.xmax > (rectangle.xmax - right_side_radius)) {
			right_side_radius *= (indicator_rect.xmax + right_side_radius - rectangle.xmax) / right_side_radius;
		}
		else {
			roundbox_corners &= ~(TOP_RIGHT | BOTTOM_RIGHT);
		}

		painter.enableGradient(
		            widget_style.decorationColor(state),
		            // shadeTop/Bottom intentionally inverted
		            widget_style.shadeBottom(state), widget_style.shadeTop(state),
		            bwGradient::DIRECTION_TOP_BOTTOM);
		painter.drawRoundbox(
		            indicator_offset_rect,
		            roundbox_corners & ~(TOP_RIGHT | BOTTOM_RIGHT),
		            widget_style.roundbox_radius);
		painter.drawRoundbox(indicator_rect,
		                     roundbox_corners & ~(TOP_LEFT | BOTTOM_LEFT),
		                     right_side_radius);
	}

	// Outline
	painter.setActiveColor(widget_style.outlineColor(state));
	painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawRoundbox(rectangle, widget_style.roundbox_corners, widget_style.roundbox_radius);

	painter.setActiveColor(widget_style.textColor(state));
	if (!is_text_editing) {
		painter.drawText(text, rectangle, widget_style.text_alignment);
	}
	painter.drawText(valueToString(2), rectangle, is_text_editing ? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT);
}

void bwNumberSlider::mousePressEvent(const MouseButton button)
{
	if (button == MOUSE_BUTTON_LEFT) {
		initial_value = value;
	}
	else if (button == MOUSE_BUTTON_RIGHT) {
		if (is_text_editing) {
			endTextEditing();
		}
		else if (is_dragging) {
			value = initial_value;
		}
	}
	is_dragging = true;
}

void bwNumberSlider::mouseReleaseEvent(const MouseButton /*button*/)
{
	is_dragging = false;
}

void bwNumberSlider::mouseClickEvent(const MouseButton button)
{
	if (button == MOUSE_BUTTON_LEFT) {
		startTextEditing();
	}
}

void bwNumberSlider::mouseDragEvent(const MouseButton button, const int drag_distance)
{
	if (button == MOUSE_BUTTON_LEFT) {
		setValue(initial_value + (drag_distance / (float)rectangle.width()));
	}
}

void bwNumberSlider::setValue(float _value)
{
	value = std::max(min, std::min(max, _value));
}

void bwNumberSlider::setMinMax(float _min, float _max)
{
	min = _min;
	max = _max;
}

std::string bwNumberSlider::valueToString(unsigned int precision) const
{
	std::stringstream string_stream;
	string_stream << std::fixed << std::setprecision(precision) << value;
	return string_stream.str();
}

float bwNumberSlider::calcValueIndicatorWidth(bwStyle& style) const
{
	const float range = max - min;

	assert(max > min);
	return ((value - min) * (rectangle.width() - style.widget_styles[type].roundbox_radius)) / range;
}
