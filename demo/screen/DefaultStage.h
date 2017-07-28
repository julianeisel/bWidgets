#pragma once

#include "Stage.h"

namespace bWidgetDemo {

class DefaultStage : public Stage
{
public:
	DefaultStage(unsigned int width, unsigned int height);

	void handleResizeEvent(const Window& win);

private:
	void updateButtonPositions();
	void iterWidgetPositions(
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
};

} // namespace bWidgetDemo
