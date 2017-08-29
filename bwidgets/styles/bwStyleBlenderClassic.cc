#include <assert.h>

#include "bwAbstractButton.h"
#include "bwStyleManager.h"
#include "bwTextBox.h"

#include "bwStyleBlenderClassic.h"

using namespace bWidgets;


const static bwWidgetStyle static_widget_styles[bwWidget::WIDGET_TYPE_TOT] = {
	[bwWidget::WIDGET_TYPE_LABEL] = {
		.state_colors = {
			[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT] = {
				.normal = 0.0f,
			},
		},
	},

	[bwWidget::WIDGET_TYPE_PUSH_BUTTON] = {
		.state_colors = {
			[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {
				.normal = 0.6f,
				.highlighted = bwColor(0.6f).shade(0.06f),
				.sunken = 0.353f,
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT] = {
				.normal = 0.0f,
				.highlighted = 0.0f,
				.sunken = 1.0f,
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE] = {
				.normal = 0.098f,
				.highlighted = 0.098f,
				.sunken = 0.098f,
			},
		},
		.shade_top = 15,
		.shade_bottom = -15,
		.text_alignment = TEXT_ALIGN_LEFT,
		.roundbox_radius = 5.0f,
	},

	[bwWidget::WIDGET_TYPE_RADIO_BUTTON] = {
		.state_colors = {
			[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {
				.normal = 0.27451f,
				.highlighted = bwColor(0.27451f).shade(0.06f),
				.sunken = bwColor(0.337255f, 0.501961f, 0.760784f),
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT] = {
				.normal = 1.0f,
				.highlighted = 1.0f,
				.sunken = 0.0f,
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE] = {
				.normal = 0.0f,
				.highlighted = 0.0f,
				.sunken = 0.0f,
			},
		},
		.shade_top = 15,
		.shade_bottom = -15,
		.text_alignment = TEXT_ALIGN_CENTER,
		.roundbox_radius = 4.0f,
	},

	[bwWidget::WIDGET_TYPE_TEXT_BOX] = {
		.state_colors = {
			[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {
				.normal = 0.6f,
				.highlighted = bwColor(0.6f).shade(0.06f),
				.sunken = 0.6f,
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT] = {
				.normal = 0.0f,
				.highlighted = 0.0f,
				.sunken = 1.0f,
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE] = {
				.normal = 0.098f,
				.highlighted = 0.098f,
				.sunken = 0.098f,
			},
			[bwWidgetStyle::WIDGET_STYLE_COLOR_DECORATION] = {
				.normal = 0.353f,
				.highlighted = 0.353f,
				.sunken = 0.353f,
			},
		},
		.shade_top = 0,
		.shade_bottom = 25,
		.text_alignment = TEXT_ALIGN_LEFT,
		.roundbox_radius = 4.0f,
	},
};

bwStyleBlenderClassic::bwStyleBlenderClassic() :
    bwStyle(STYLE_BLENDER_CLASSIC, static_widget_styles)
{
	
}

void bwStyleBlenderClassic::setWidgetStyle(const bwWidget& widget)
{
	const bwAbstractButton* button = widget_cast<const bwAbstractButton*>(&widget);

	if (button) {
		widget_styles[widget.type].roundbox_corners = button->rounded_corners;
	}
	else if (widget.type == bwWidget::WIDGET_TYPE_TEXT_BOX) {
		widget_styles[widget.type].roundbox_corners = RoundboxCorner::ALL;
	}
}
