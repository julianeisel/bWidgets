#include <assert.h>

#include "bwAbstractButton.h"
#include "bwPainter.h"
#include "bwPanel.h"
#include "bwStyleManager.h"
#include "bwTextBox.h"

#include "bwStyleClassic.h"

using namespace bWidgets;


static void widget_base_style_checkbox_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	r_base_style.background_color = 0.27451f;
	r_base_style.text_color = 0.0f;
	r_base_style.border_color = 0.0f;
	r_base_style.decoration_color = 1.0f;
	r_base_style.shade_top = 15;
	r_base_style.shade_bottom = -15;
	r_base_style.corner_radius = 4.0f;

	if (widget.state == bwWidget::STATE_HIGHLIGHTED) {
		r_base_style.background_color.shade(0.06f);
	}
	else if (widget.state == bwWidget::STATE_SUNKEN) {
		r_base_style.text_color = 1.0f;
		std::swap(r_base_style.shade_top, r_base_style.shade_bottom);
	}
}
static void widget_base_style_number_slider_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	r_base_style.background_color = 180u;
	r_base_style.text_color = 0.0f;
	r_base_style.border_color = 25u;
	r_base_style.decoration_color = 128u;
	r_base_style.shade_top = -20;
	r_base_style.shade_bottom = 0;
	r_base_style.corner_radius = 10.0f;

	if (widget.state == bwWidget::STATE_HIGHLIGHTED) {
		r_base_style.background_color.shade(0.06f);
	}
	else if (widget.state == bwWidget::STATE_SUNKEN) {
		r_base_style.text_color = 1.0f;
		r_base_style.background_color = 153u;
	}
}
static void widget_base_style_push_button_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	r_base_style.background_color = 0.6f;
	r_base_style.text_color = 0.0f;
	r_base_style.border_color = 0.098f;
	r_base_style.shade_top = 15;
	r_base_style.shade_bottom = -15;
	r_base_style.corner_radius = 5.0f;

	if (widget.state == bwWidget::STATE_HIGHLIGHTED) {
		r_base_style.background_color.shade(0.06f);
	}
	else if (widget.state == bwWidget::STATE_SUNKEN) {
		r_base_style.background_color = 0.353f;
		r_base_style.text_color = 1.0f;
		std::swap(r_base_style.shade_top, r_base_style.shade_bottom);
	}
}
static void widget_base_style_radio_button_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	r_base_style.background_color = 0.27451f;
	r_base_style.text_color = 1.0f;
	r_base_style.border_color = 0.0f;
	r_base_style.shade_top = 15;
	r_base_style.shade_bottom = -15;
	r_base_style.text_alignment = TEXT_ALIGN_CENTER;
	r_base_style.corner_radius = 4.0f;

	if (widget.state == bwWidget::STATE_HIGHLIGHTED) {
		r_base_style.background_color.shade(0.06f);
	}
	else if (widget.state == bwWidget::STATE_SUNKEN) {
		r_base_style.background_color = bwColor(0.337255f, 0.501961f, 0.760784f);
		r_base_style.text_color = 0.0f;
		std::swap(r_base_style.shade_top, r_base_style.shade_bottom);
	}
}
static void widget_base_style_scroll_bar_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	r_base_style.background_color = bwColor(80u, 180u);
	r_base_style.text_color = 0.0f;
	r_base_style.border_color = bwColor(50u, 180u);
	r_base_style.decoration_color = bwColor(128u);
	r_base_style.shade_top = 5;
	r_base_style.shade_bottom = -5;
	r_base_style.corner_radius = 6.5f;

	if (widget.state == bwWidget::STATE_SUNKEN) {
		r_base_style.decoration_color.shade(5u);
		r_base_style.text_color = 1.0f;
	}
}
static void widget_base_style_text_box_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	r_base_style.background_color = 0.6f;
	r_base_style.text_color = 0.0f;
	r_base_style.border_color = 0.098f;
	r_base_style.decoration_color = 0.353f;
	r_base_style.shade_top = 0;
	r_base_style.shade_bottom = 25;
	r_base_style.corner_radius = 4.0f;

	if (widget.state == bwWidget::STATE_HIGHLIGHTED) {
		r_base_style.background_color.shade(0.06f);
	}
	else if (widget.state == bwWidget::STATE_SUNKEN) {
		r_base_style.text_color = 1.0f;
	}
}
static void widget_base_style_panel_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	bwPanel* panel = widget_cast<bwPanel*>(&widget);

	r_base_style.background_color = 114u;
	r_base_style.border_color = 114u;

	panel->draw_separator = true;
}

static void widget_style_properties_set_to_default(
        bwWidget& widget)
{
	for (const bwPointer<bwStyleProperty>& property : widget.style_properties) {
		property->setValueToDefault();
	}
}

static void widget_base_style_set(
        bwWidget& widget,
        bwWidgetBaseStyle& r_base_style)
{
	switch (widget.type) {
		case bwWidget::WIDGET_TYPE_CHECKBOX:
			widget_base_style_checkbox_set(widget, r_base_style);
			break;
		case bwWidget::WIDGET_TYPE_NUMBER_SLIDER:
			widget_base_style_number_slider_set(widget, r_base_style);
			break;
		case bwWidget::WIDGET_TYPE_PUSH_BUTTON:
			widget_base_style_push_button_set(widget, r_base_style);
			break;
		case bwWidget::WIDGET_TYPE_RADIO_BUTTON:
			widget_base_style_radio_button_set(widget, r_base_style);
			break;
		case bwWidget::WIDGET_TYPE_SCROLL_BAR:
			widget_base_style_scroll_bar_set(widget, r_base_style);
			break;
		case bwWidget::WIDGET_TYPE_TEXT_BOX:
			widget_base_style_text_box_set(widget, r_base_style);
			break;
		case bwWidget::WIDGET_TYPE_PANEL:
			widget_base_style_panel_set(widget, r_base_style);
			break;
		default:
			break;
	}
}

bwStyleClassic::bwStyleClassic() :
    bwStyle(STYLE_CLASSIC)
{
	
}

void bwStyleClassic::setWidgetStyle(bwWidget& widget)
{
	bwWidgetBaseStyle* base_style = nullptr;

	polish(widget);

	if (bwAbstractButton* button = widget_cast<bwAbstractButton*>(&widget)) {
		base_style = &button->base_style;
		base_style->roundbox_corners = button->rounded_corners;
	}
	else if (bwPanel* panel = widget_cast<bwPanel*>(&widget)) {
		base_style = &panel->base_style;
	}
	else if (bwTextBox* text_box = widget_cast<bwTextBox*>(&widget)) {
		base_style = &text_box->base_style;
		base_style->roundbox_corners = RoundboxCorner::ALL; // XXX Incorrect, should set this in layout.
	}
	else {
//		base_style->roundbox_corners = RoundboxCorner::ALL;
	}

	widget_style_properties_set_to_default(widget);

	if (base_style) {
		widget_base_style_set(widget, *base_style);
	}
}
