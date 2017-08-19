#pragma once

#include "Stage.h"

namespace bWidgetsDemo {

class DefaultStage : public Stage
{
public:
	DefaultStage(unsigned int width, unsigned int height);

	void handleResizeEvent(const Window& win);

private:
	void updateButtonPositions();
	float iterWidgetPositions(
	        const unsigned int offset_top,
	        void (*callback)(Stage&, const unsigned int, const unsigned int,
	                         const unsigned int, const unsigned int, const unsigned int));

	static void widgetAddCb(
	        Stage&, const unsigned int,
	        const unsigned int, const unsigned int,
	        const unsigned int, const unsigned int);
	static void updateWidgetPositionCb(
	        Stage&, const unsigned int,
	        const unsigned int, const unsigned int,
	        const unsigned int, const unsigned int);
	static void StyleApplyButtonCb(bWidgets::bwWidget& widget);

	const unsigned int padding = 10;
};

} // namespace bWidgetsDemo
