#include <assert.h>

#include "bwAbstractButton.h"
#include "bwPainter.h"
#include "bwStyleManager.h"
#include "bwTextBox.h"

#include "bwStyleBlenderClassic.h"

using namespace bWidgets;


static bwStyle::bwWidgetStyleArray static_widget_styles;


static void initWidgetStyles(bwStyleBlenderClassic& style)
{
	bwWidgetStyle* widget_style;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_CHECKBOX];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {0.27451f, bwColor(0.27451f).shade(0.06f), 0.27451f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT]       = {0.0f, 0.0f, 1.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {0.0f, 0.0f, 0.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_DECORATION] = {1.0f, 1.0f, 1.0f};
	widget_style->shade_top = 15;
	widget_style->shade_bottom = -15;
	widget_style->invert_shade_on_sunken = true;
	widget_style->roundbox_radius = 4.0f;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_NUMBER_SLIDER];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {0.6f, bwColor(0.6f).shade(0.06f), 0.6f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT]       = {0.0f, 0.0f, 1.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {0.098f, 0.098f, 0.098f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_DECORATION] = {0.353f, 0.353f, 0.353f};
	widget_style->shade_top = 0;
	widget_style->shade_bottom = 25;
	widget_style->invert_shade_on_sunken = true;
	widget_style->roundbox_radius = 10.0f;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_PUSH_BUTTON];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {0.6f, bwColor(0.6f).shade(0.06f), 0.353f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT]       = {0.0f, 0.0f, 1.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {0.098f, 0.098f, 0.098f};
	widget_style->shade_top = 15;
	widget_style->shade_bottom = -15;
	widget_style->invert_shade_on_sunken = true;
	widget_style->roundbox_radius = 5.0f;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_RADIO_BUTTON];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {0.27451f, bwColor(0.27451f).shade(0.06f), bwColor(0.337255f, 0.501961f, 0.760784f)};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT]       = {1.0f, 1.0f, 0.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {0.0f, 0.0f, 0.0f};
	widget_style->shade_top = 15;
	widget_style->shade_bottom = -15;
	widget_style->invert_shade_on_sunken = true;
	widget_style->text_alignment = TEXT_ALIGN_CENTER;
	widget_style->roundbox_radius = 4.0f;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_SCROLL_BAR];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {bwColor(80u, 180u), bwColor(80u, 180u).shade(0.06f), bwColor(100u, 180u)};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT]       = {0.0f, 0.0f, 1.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {bwColor(50u, 180u), bwColor(50u) , bwColor(50u, 180u)};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_DECORATION] = {bwColor(128u), bwColor(128u) , bwColor(128u).shade(0.06f)};
	widget_style->shade_top = 5;
	widget_style->shade_bottom = -5;
	widget_style->roundbox_radius = 6.5f;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_TEXT_BOX];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {0.6f, bwColor(0.6f).shade(0.06f), 0.6f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_TEXT]       = {0.0f, 0.0f, 1.0f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {0.098f, 0.098f, 0.098f};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_DECORATION] = {0.353f, 0.353f, 0.353f};
	widget_style->shade_top = 0;
	widget_style->shade_bottom = 25;
	widget_style->roundbox_radius = 4.0f;

	widget_style = &style.widget_styles[bwWidget::WIDGET_TYPE_PANEL];
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_BACKGROUND] = {114u};
	widget_style->state_colors[bwWidgetStyle::WIDGET_STYLE_COLOR_OUTLINE]    = {114u};
}

bwStyleBlenderClassic::bwStyleBlenderClassic() :
    bwStyle(STYLE_BLENDER_CLASSIC, static_widget_styles)
{
	draw_panel_separators = true;
	initWidgetStyles(*this);
}

void bwStyleBlenderClassic::setWidgetStyle(const bwWidget& widget)
{
	if (const bwAbstractButton* button = widget_cast<const bwAbstractButton*>(&widget)) {
		widget_styles[widget.type].roundbox_corners = button->rounded_corners;
	}
	else {
		widget_styles[widget.type].roundbox_corners = RoundboxCorner::ALL;
	}
}
