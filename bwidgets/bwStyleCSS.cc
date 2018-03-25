#include "bwAbstractButton.h"
#include "bwPainter.h"

#include "bwStyleCSS.h"

using namespace bWidgets;


void(* bwStyleCSS::polish_cb)(class bwWidget&) = nullptr;


bwStyleCSS::bwStyleCSS() :
    bwStyle(STYLE_CLASSIC_CSS)
{
	
}

#include "bwTextBox.h" // XXX Ugly
#include "bwPanel.h"
void bwStyleCSS::setWidgetStyle(bwWidget& widget)
{
	bwWidgetBaseStyle* base_style = nullptr;

	polish(widget);

	if (bwAbstractButton* button = widget_cast<bwAbstractButton*>(&widget)) {
		base_style = &button->base_style;
		base_style->roundbox_corners = button->rounded_corners;
	}
	else if (bwPanel* panel = widget_cast<bwPanel*>(&widget)) {
		base_style = &panel->base_style;
		base_style->roundbox_corners = RoundboxCorner::ALL;
	}
	else if (bwTextBox* text_box = widget_cast<bwTextBox*>(&widget)) {
		base_style = &text_box->base_style;
		base_style->roundbox_corners = RoundboxCorner::ALL; // XXX Incorrect, should set this in layout.
	}
	else {
//		base_style->roundbox_corners = RoundboxCorner::ALL;
	}
}

void bwStyleCSS::polish(bwWidget& widget)
{
	if (polish_cb) {
		polish_cb(widget);
	}
}
