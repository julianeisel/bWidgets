#include "PushButton.h"
#include "Stage.h"

#include "DefaultStage.h"

using namespace bWidgetDemo;


DefaultStage::DefaultStage(unsigned int width, unsigned int height) :
    Stage(width, height)
{
	iterWidgetPositions(widgetAddCb);
}

void DefaultStage::updateButtonPositions()
{
	iterWidgetPositions(updateWidgetPositionCb);
}

void DefaultStage::handleResizeEvent(const Window &win)
{
	Stage::handleResizeEvent(win);
	updateButtonPositions();
}

void DefaultStage::iterWidgetPositions(
        void (*callback)(Stage& stage, const unsigned int index,
                         const unsigned int xmin, const unsigned int ymin,
                         const unsigned int width, const unsigned int height))
{
	const unsigned int but_width = 130;
	const unsigned int but_height = 20;
	const unsigned int stage_width = getWidth();
	const unsigned int stage_height = getHeight();
	const unsigned int pad = 10;
	int ofs_left = 0;
	int ofs_top = 0;

	for (unsigned int i = 0; i < 20; i++) {
//		const int xmin = pad + (i * (pad + but_width));
		const int xmin = pad + ofs_left;
		const int ymin = stage_height - pad - but_height - ofs_top;

		callback(*this, i, xmin, ymin, but_width, but_height);

		if (i == 9) {
			ofs_left += stage_width - (2 * pad) - but_width;
			ofs_top = 0;
		}
		else {
			ofs_top += but_height + pad;
		}
	}
}

void DefaultStage::widgetAddCb(
        Stage& stage, const unsigned int index,
        const unsigned int xmin, const unsigned int ymin,
        const unsigned int width, const unsigned int height)
{
	stage.widgetAdd(new bWidgets::PushButton("Widget " + std::to_string(index), xmin, ymin, width, height));
}

void DefaultStage::updateWidgetPositionCb(
        Stage& stage, const unsigned int index,
        const unsigned int xmin, const unsigned int ymin,
        const unsigned int width, const unsigned int height)
{
	bWidgets::Widget& widget = stage.getWidgetAt(index);

	widget.pos_x = xmin;
	widget.pos_y = ymin;
	widget.width = width;
	widget.height = height;
}
