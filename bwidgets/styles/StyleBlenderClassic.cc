#include "Painter.h"
#include "Widget.h"

#include "StyleBlenderClassic.h"

using namespace bWidgets;


void StyleBlenderClassic::setWidgetStyleDefault(const Widget& widget)
{
	widget_style.outline_color.setColor(0.098f);

	if (widget.state == Widget::STATE_NORMAL || widget.state == Widget::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.6f);
		widget_style.text_color.setColor(0.0f);
		widget_style.shade_top = 15;
		widget_style.shade_bottom = -15;

		if (widget.state == Widget::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (widget.state == Widget::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.353f);
		widget_style.text_color.setColor(1.0f);
		widget_style.shade_top = -15;
		widget_style.shade_bottom = 15;
	}

	widget_style.roundbox_radius = 5.0f;
	widget_style.roundbox_corners = RoundboxCorner::ALL;
}

void StyleBlenderClassic::setWidgetStyleLabel()
{
	widget_style.text_color.setColor(0.0f);
}

void StyleBlenderClassic::setWidgetStyleRadioButton(const Widget& widget)
{
	widget_style.outline_color.setColor(0.0f);

	if (widget.state == Widget::STATE_NORMAL || widget.state == Widget::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.27451f);
		widget_style.text_color.setColor(1.0f);
		widget_style.shade_top = 15;
		widget_style.shade_bottom = -15;

		if (widget.state == Widget::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (widget.state == Widget::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.337255f, 0.501961f, 0.760784f);
		widget_style.text_color.setColor(0.0f);
		widget_style.shade_top = -15;
		widget_style.shade_bottom = 15;
	}

	widget_style.roundbox_radius = 4.0f;
	widget_style.roundbox_corners = RoundboxCorner::ALL;
}

void StyleBlenderClassic::setWidgetStyle(const Widget& widget)
{
	switch (widget.type) {
		case Widget::WIDGET_TYPE_PUSH_BUTTON:
			setWidgetStyleDefault(widget);
			break;
		case Widget::WIDGET_TYPE_RADIO_BUTTON:
			setWidgetStyleRadioButton(widget);
			break;
		case Widget::WIDGET_TYPE_LABEL:
			setWidgetStyleLabel();
			break;
	}
}
