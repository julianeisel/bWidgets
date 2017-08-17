#include "bwStyleManager.h"
#include "bwAbstractButton.h"

#include "bwStyleBlenderClassic.h"

using namespace bWidgets;


bwStyleBlenderClassic::bwStyleBlenderClassic() :
    bwStyle(STYLE_BLENDER_CLASSIC)
{
	
}

void bwStyleBlenderClassic::setWidgetStyleDefault(const bwWidget& widget)
{
	const bwAbstractButton& button = widget_cast<const bwAbstractButton&>(widget);

	widget_style.outline_color.setColor(0.098f);

	if (button.state == bwAbstractButton::STATE_NORMAL || button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.6f);
		widget_style.text_color.setColor(0.0f);
		widget_style.shade_top = 15;
		widget_style.shade_bottom = -15;

		if (button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (button.state == bwAbstractButton::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.353f);
		widget_style.text_color.setColor(1.0f);
		widget_style.shade_top = -15;
		widget_style.shade_bottom = 15;
	}

	widget_style.roundbox_radius = 5.0f;
	widget_style.roundbox_corners = button.rounded_corners;
}

void bwStyleBlenderClassic::setWidgetStyleRadioButton(const bwWidget& widget)
{
	const bwAbstractButton& button = widget_cast<const bwAbstractButton&>(widget);

	widget_style.outline_color.setColor(0.0f);

	if (button.state == bwAbstractButton::STATE_NORMAL || button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.27451f);
		widget_style.text_color.setColor(1.0f);
		widget_style.shade_top = 15;
		widget_style.shade_bottom = -15;

		if (button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (button.state == bwAbstractButton::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.337255f, 0.501961f, 0.760784f);
		widget_style.text_color.setColor(0.0f);
		widget_style.shade_top = -15;
		widget_style.shade_bottom = 15;
	}

	widget_style.roundbox_radius = 4.0f;
	widget_style.roundbox_corners = button.rounded_corners;
}

void bwStyleBlenderClassic::setWidgetStyleLabel()
{
	widget_style.text_color.setColor(0.0f);
}

void bwStyleBlenderClassic::setWidgetStyle(const bwWidget& widget)
{
	switch (widget.type) {
		case bwWidget::WIDGET_TYPE_PUSH_BUTTON:
			setWidgetStyleDefault(widget);
			break;
		case bwWidget::WIDGET_TYPE_RADIO_BUTTON:
			setWidgetStyleRadioButton(widget);
			break;
		case bwWidget::WIDGET_TYPE_LABEL:
			setWidgetStyleLabel();
			break;
	}
}
