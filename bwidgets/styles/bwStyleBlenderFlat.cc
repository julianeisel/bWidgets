#include <cassert>

#include "bwAbstractButton.h"
#include "bwTextBox.h"

#include "bwStyleBlenderFlat.h"

using namespace bWidgets;


bwStyleBlenderFlat::bwStyleBlenderFlat() :
    bwStyle(STYLE_BLENDER_FLAT)
{
	
}

void bwStyleBlenderFlat::setWidgetStyleDefault(const bwWidget& widget)
{
	const bwAbstractButton& button = *widget_cast<const bwAbstractButton*>(&widget);

	if (button.state == bwAbstractButton::STATE_NORMAL || button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.6f);
		widget_style.outline_color.setColor(0.6f);
		widget_style.text_color.setColor(0.0f);

		if (button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (button.state == bwAbstractButton::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.353f);
		widget_style.outline_color.setColor(0.353f);
		widget_style.text_color.setColor(1.0f);
	}

	widget_style.shade_top = widget_style.shade_bottom = 0;
	widget_style.roundbox_radius = 3.0f;
	widget_style.roundbox_corners = button.rounded_corners;
	widget_style.text_alignment = TEXT_ALIGN_LEFT;
}

void bwStyleBlenderFlat::setWidgetStyleRadioButton(const bwWidget& widget)
{
	const bwAbstractButton& button = *widget_cast<const bwAbstractButton*>(&widget);

	if (button.state == bwAbstractButton::STATE_NORMAL || button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
		widget_style.fill_color.setColor(0.27451f);
		widget_style.outline_color.setColor(0.27451f);
		widget_style.text_color.setColor(1.0f);

		if (button.state == bwAbstractButton::STATE_HIGHLIGHTED) {
			widget_style.fill_color.shade(0.06f);
		}
	}
	else if (button.state == bwAbstractButton::STATE_SUNKEN) {
		widget_style.fill_color.setColor(0.337255f, 0.501961f, 0.760784f);
		widget_style.outline_color.setColor(0.337255f, 0.501961f, 0.760784f);
		widget_style.text_color.setColor(0.0f);
	}

	widget_style.shade_top = widget_style.shade_bottom = 0;
	widget_style.roundbox_radius = 2.0f;
	widget_style.roundbox_corners = button.rounded_corners;
	widget_style.text_alignment = TEXT_ALIGN_CENTER;
}

void bwStyleBlenderFlat::setWidgetStyleLabel()
{
	widget_style.shade_top = widget_style.shade_bottom = 0;
	widget_style.text_color.setColor(0.0f);
	widget_style.text_alignment = TEXT_ALIGN_LEFT;
}

void bwStyleBlenderFlat::setWidgetStyleTextBox(const bwWidget& widget)
{
	const bwTextBox& text_box = *widget_cast<const bwTextBox*>(&widget);

	widget_style.outline_color.setColor(0.6f);
	widget_style.fill_color.setColor(0.6f);
	widget_style.text_color.setColor(0.0f);
	widget_style.highlight.setColor(0.353f);
	if (text_box.state == bwTextBox::STATE_HIGHLIGHTED) {
		widget_style.fill_color.shade(0.06f);
	}
	else if (text_box.state == bwTextBox::STATE_TEXT_EDITING) {
		widget_style.text_color.setColor(1.0f);
	}

	widget_style.shade_top = widget_style.shade_bottom = 0;
	widget_style.roundbox_radius = 4.0f;
	widget_style.roundbox_corners = RoundboxCorner::ALL;
	widget_style.text_alignment = TEXT_ALIGN_LEFT;
}

void bwStyleBlenderFlat::setWidgetStyle(const bwWidget& widget)
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
		case bwWidget::WIDGET_TYPE_TEXT_BOX:
			setWidgetStyleTextBox(widget);
			break;
		default:
			assert(0);
			break;
	}
}
