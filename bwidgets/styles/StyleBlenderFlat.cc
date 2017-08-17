#include "Widget.h"

#include "StyleBlenderFlat.h"

using namespace bWidgets;


StyleBlenderFlat::StyleBlenderFlat() :
    Style(STYLE_BLENDER_FLAT)
{
	
}

void StyleBlenderFlat::setWidgetStyleDefault(const Widget& widget)
{
	if (widget.state == Widget::STATE_NORMAL || widget.state == Widget::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.6f);
		widget_style.outline_color.setColor(0.6f);
		widget_style.text_color.setColor(0.0f);

		if (widget.state == Widget::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (widget.state == Widget::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.353f);
		widget_style.outline_color.setColor(0.353f);
		widget_style.text_color.setColor(1.0f);
	}

	widget_style.roundbox_radius = 3.0f;
	widget_style.roundbox_corners = widget.rounded_corners;
}

void StyleBlenderFlat::setWidgetStyleLabel()
{
	widget_style.text_color.setColor(0.0f);
}

void StyleBlenderFlat::setWidgetStyleRadioButton(const Widget& widget)
{

	if (widget.state == Widget::STATE_NORMAL || widget.state == Widget::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.27451f);
		widget_style.outline_color.setColor(0.27451f);
		widget_style.text_color.setColor(1.0f);

		if (widget.state == Widget::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (widget.state == Widget::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.337255f, 0.501961f, 0.760784f);
		widget_style.outline_color.setColor(0.337255f, 0.501961f, 0.760784f);
		widget_style.text_color.setColor(0.0f);
	}

	widget_style.roundbox_radius = 2.0f;
	widget_style.roundbox_corners = widget.rounded_corners;
}

void StyleBlenderFlat::setWidgetStyle(const Widget& widget)
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
