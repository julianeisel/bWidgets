#include <cassert>
#include <cmath>
#include <iostream>

#include "bwPainter.h"
#include "bwPanel.h"
#include "bwPolygon.h"
#include "bwStyle.h"

using namespace bWidgets;


bwPanel::bwPanel(
        const std::string& label,
        unsigned int header_height_hint) :
    bwWidget(WIDGET_TYPE_PANEL, 0, header_height_hint),
    header_height(header_height_hint),
    label(label)
{
	
}

void bwPanel::draw(bwStyle& style) const
{
	const bwWidgetStyle& widget_style = style.widget_styles[type];
	bwPainter painter;

	style.setWidgetStyle(*this);

	painter.drawRoundboxWidgetBase(widget_style, state, style, rectangle);

	drawHeader(style);
}

void bwPanel::mousePressEvent(
        const bwWidget::MouseButton button,
        const bwPoint& location)
{
	if ((button != MOUSE_BUTTON_LEFT) || !isCoordinateInsideHeader(location)) {
		// Skip
	}
	else if (panel_state == PANEL_CLOSED) {
		panel_state = PANEL_OPEN;
	}
	else if (panel_state == PANEL_OPEN) {
		panel_state = PANEL_CLOSED;
	}
	else {
		assert(0);
	}
}

bool bwPanel::isCoordinateInsideHeader(const bwPoint& point) const
{
	const bwRectanglePixel header_rect = getHeaderRectangle();
	return header_rect.isCoordinateInside(point.x, point.y);
}

unsigned int bwPanel::getHeaderHeightHint() const
{
	return height_hint;
}

// TODO GPL!
static void panel_draw_drag_dots(
        bwPainter& painter,
        const bwRectanglePixel& rectangle,
        const bwWidgetStyle& widget_style,
        bwWidget::WidgetState state)
{
	const int px = 1.0f; // TODO Equivalent to U.pixelsize.
	const int px_zoom = std::max((int)std::round(rectangle.height() / 22.0f), 1);
	const float tint = 84.0f / 255.0f;

	const int padding    = std::max((int)std::round(rectangle.height() / 3.0f), px);
	const int dot_margin = std::max((int)std::round(px_zoom * 2.0f), px);
	const int dot_size   = std::max((int)std::round((rectangle.height() / 8.0f) - px), px);

	bwColor dot_color    = widget_style.backgroundColor(state);
	bwColor shadow_color = widget_style.backgroundColor(state);


	dot_color.shade(tint);
	shadow_color.shade(-tint);

	painter.active_drawtype = bwPainter::DRAW_TYPE_FILLED;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 2; row++) {
			const bwPoint pos{
			    ((float)rectangle.xmin + padding) + (col * (dot_size + dot_margin)),
			    ((float)rectangle.ymin + padding) + (row * (dot_size + dot_margin))
			};
			bwRectanglePixel dot_rect{(int)pos.x - dot_size, (int)pos.x, (int)pos.y, (int)pos.y + dot_size};
			bwRectanglePixel shadow_rect{dot_rect};

			shadow_rect.ymin -= dot_size;
			shadow_rect.ymax -= dot_size;

			painter.setActiveColor(shadow_color);
			painter.drawRectangle(shadow_rect);
			painter.setActiveColor(dot_color);
			painter.drawRectangle(dot_rect);
		}
	}
}

void bwPanel::drawHeader(bwStyle& style) const
{
	const bwWidgetStyle& widget_style = style.widget_styles[type];
	bwRectanglePixel header_rect = getHeaderRectangle();
	bwRectanglePixel text_rect = header_rect;
	bwRectangle<float> drag_rect = header_rect;
	bwRectangle<float> triangle_rect = header_rect; // Using float makes the triangle slightly bigger, matching Blender.
	bwPainter painter;

	// TODO How the panel header will eventually be handled isn't entirely sure
	// yet. It may become its own widget, or the layout adds the needed widgets
	// for it (e.g. bwLabel for the panel label). For now, let bwPanel draw the
	// label itself.

	if (style.draw_panel_separators) {
		painter.setActiveColor(bwColor(0.0f, 0.5f));
		painter.drawLine(bwPoint(header_rect.xmin, header_rect.ymax),
		                 bwPoint(header_rect.xmax, header_rect.ymax));
		painter.setActiveColor(bwColor(1.0f, 0.25f));
		painter.drawLine(bwPoint(header_rect.xmin, header_rect.ymax - 1),
		                 bwPoint(header_rect.xmax, header_rect.ymax - 1));
	}

	text_rect.xmin += (20 * style.dpi_fac) + 5;
	text_rect.xmin -= 10; // XXX Demo app adds 10px when drawing text.
	painter.setActiveColor(widget_style.textColor(state));
	painter.drawText(label, text_rect, widget_style.text_alignment);

	drag_rect.xmax -= 5.0f;
	drag_rect.xmin = drag_rect.xmax - drag_rect.height();
	drag_rect.scale(0.7f);
	panel_draw_drag_dots(painter, drag_rect, widget_style, state);

	triangle_rect.xmin += 5;
	triangle_rect.xmax = triangle_rect.xmin + triangle_rect.height();
	triangle_rect.scale(0.35f);
	painter.active_drawtype = bwPainter::DRAW_TYPE_FILLED;
	painter.use_antialiasing = true;
	painter.setActiveColor(widget_style.textColor(state));
	painter.drawTriangle(triangle_rect, (panel_state == PANEL_OPEN) ? DIRECTION_DOWN : DIRECTION_RIGHT);
}

bwRectanglePixel bwPanel::getHeaderRectangle() const
{
	bwRectanglePixel header_rect{rectangle};
	header_rect.ymin = header_rect.ymax - header_height;
	return header_rect;
}
