#include "Label.h"
#include "PushButton.h"
#include "RadioButton.h"
#include "Stage.h"

#include "DefaultStage.h"

using namespace bWidgetDemo;


DefaultStage::DefaultStage(unsigned int width, unsigned int height) :
    Stage(width, height)
{
	const unsigned int stage_height = getHeight();
	const unsigned int button_width = 80;
	const unsigned int button_height = 20;
	unsigned int offset_top = 0;

	for (int i = 0; i < 1; i++) {
		const unsigned int ymin = stage_height - padding - button_height;
		bWidgets::Widget* style_button = new bWidgets::RadioButton(
		                                     "Temp", padding, ymin,
		                                     button_width, button_height);

		offset_top += button_height + padding;
		widgetAdd(style_button);
	}

	iterWidgetPositions(offset_top, widgetAddCb);
}

void DefaultStage::updateButtonPositions()
{
	const unsigned int button_width = 20;
	unsigned int offset_top = 0;

	for (int i = 0; i < 1; i++) {
		offset_top += padding + button_width;
	}

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
