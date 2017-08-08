#include "Painter.h"
#include "Widget.h"

#include "StyleBlenderClassic.h"

using namespace bWidgets;


void StyleBlenderClassic::setWidgetStyle(const Widget& widget)
{
	widget_style.outline_color.setColor(0.098f);

	if (widget.state == Widget::STATE_NORMAL || widget.state == Widget::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.6f);
		widget_style.text_color.setColor(0.0f);

		if (widget.state == Widget::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (widget.state == Widget::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.353f);
		widget_style.text_color.setColor(1.0f);
	}

	widget_style.roundbox_radius = 5.0f;
	widget_style.roundbox_corners = RoundboxCorner::ALL;
}
