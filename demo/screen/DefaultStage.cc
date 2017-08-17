#include "Font.h"
#include "Stage.h"

// bWidgets
#include "Label.h"
#include "PushButton.h"
#include "RadioButton.h"
#include "Style.h"
#include "StyleManager.h"

#include "DefaultStage.h"

using namespace bWidgetDemo;


DefaultStage::DefaultStage(unsigned int width, unsigned int height) :
    Stage(width, height)
{
	const unsigned int stage_height = getHeight();
	const unsigned int button_width = 130;
	const unsigned int button_height = 20;
	const unsigned int ymin = stage_height - padding - button_height;
	const unsigned int xmin = padding;
	unsigned int offset_xmin = 0;

	bWidgets::Widget* prev_button = nullptr;

	const std::string& style_str = "Style: ";
	const unsigned int str_width = font->calculateStringWidth(style_str);
	widgetAdd(new bWidgets::Label(style_str, xmin, ymin, str_width + padding, button_height));
	offset_xmin += str_width + padding;

	for (bWidgets::Style::StyleType type : bWidgets::StyleManager::getStyleManager().getBuiltinStyleTypes()) {
		bWidgets::RadioButton* style_button = new bWidgets::RadioButton(
		                                          type.name, xmin + offset_xmin - (prev_button ? 1 : 0), ymin,
		                                          button_width, button_height);

		style_button->valueID = type.type_id;
		style_button->custom_data = this;
		style_button->apply = StyleApplyButtonCb;

		if (type.type_id == style->type_id) {
			style_button->state = bWidgets::Widget::STATE_SUNKEN;
		}

		widgetAdd(style_button);
		offset_xmin += button_width;

		if (prev_button) {
			prev_button->rounded_corners = bWidgets::BOTTOM_LEFT | bWidgets::TOP_LEFT;
			style_button->rounded_corners = bWidgets::BOTTOM_RIGHT | bWidgets::TOP_RIGHT;
		}
		prev_button = style_button;
	}

	iterWidgetPositions(button_height + padding, widgetAddCb);
}

void DefaultStage::updateButtonPositions()
{
	const unsigned int button_width = 20;
	unsigned int offset_top = 0;

	/* style selector */
	offset_top += padding + button_width;

	iterWidgetPositions(offset_top, updateWidgetPositionCb);
}

void DefaultStage::handleResizeEvent(const Window &win)
{
	Stage::handleResizeEvent(win);
	updateButtonPositions();
}

void DefaultStage::iterWidgetPositions(
        const unsigned int offset_top,
        void (*callback)(Stage& stage, const unsigned int index,
                         const unsigned int xmin, const unsigned int ymin,
                         const unsigned int width, const unsigned int height))
{
	const unsigned int but_width = 130;
	const unsigned int but_height = 20;
	const unsigned int stage_width = getWidth();
	const unsigned int stage_height = getHeight();
	int ofs_left = 0;
	int ofs_top = offset_top;

	for (unsigned int i = 0; i < 20; i++) {
//		const int xmin = pad + (i * (pad + but_width));
		const int xmin = padding + ofs_left;
		const int ymin = stage_height - padding - but_height - ofs_top;

		callback(*this, i, xmin, ymin, but_width, but_height);

		if (i == 9) {
			ofs_left += stage_width - (2 * padding) - but_width;
			ofs_top = offset_top;
		}
		else {
			ofs_top += but_height + padding;
		}
	}
}

void DefaultStage::StyleApplyButtonCb(bWidgets::Widget& widget)
{
	bWidgets::RadioButton& radio_but = static_cast<bWidgets::RadioButton&>(widget);
	DefaultStage* stage = reinterpret_cast<DefaultStage*>(radio_but.custom_data);

	stage->activateStyleID((bWidgets::Style::StyleTypeID)radio_but.valueID);

	// Deactivate other style radio buttons
	for (bWidgets::Widget* widget_iter : stage->widgets) {
		if ((widget_iter != &widget) &&
		    (widget_iter->type == bWidgets::Widget::WIDGET_TYPE_RADIO_BUTTON))
		{
			bWidgets::RadioButton* radio_iter = reinterpret_cast<bWidgets::RadioButton*>(widget_iter);
			if (radio_iter->custom_data == radio_but.custom_data) {
				radio_iter->state = bWidgets::Widget::STATE_NORMAL;
			}
		}
	}
}

void DefaultStage::widgetAddCb(
        Stage& stage, const unsigned int index,
        const unsigned int xmin, const unsigned int ymin,
        const unsigned int width, const unsigned int height)
{
	bWidgets::Widget* widget;

	if (index < 5) {
		widget = new bWidgets::Label("Label " + std::to_string(index), xmin, ymin, width, height);
	}
	else {
		widget = new bWidgets::PushButton("Push Button " + std::to_string(index - 5), xmin, ymin, width, height);
	}
	stage.widgetAdd(widget);
}

void DefaultStage::updateWidgetPositionCb(
        Stage& stage, const unsigned int index,
        const unsigned int xmin, const unsigned int ymin,
        const unsigned int width, const unsigned int height)
{
	bWidgets::Widget& widget = *stage.getWidgetAt(index);

	widget.rectangle.xmin = xmin;
	widget.rectangle.xmax = xmin + width;
	widget.rectangle.ymin = ymin;
	widget.rectangle.ymax = ymin + height;
}
