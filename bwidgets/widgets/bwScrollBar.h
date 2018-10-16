#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwScrollBar : public bwAbstractButton
{
public:
	bwScrollBar(
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

	void draw(bwStyle &style) override;
	void mousePressEvent(
	        const MouseButton button,
	        const bwPoint& location) override;
	void mouseReleaseEvent(
	        const MouseButton button,
	        const bwPoint& location) override;
	void mouseDragEvent(
	        const MouseButton button,
	        const bwDistance drag_distance) override;

	float ratio = 0.0f; // Ration between content and area height (max 1.0f).
	int scroll_offset = 0;

private:
	int mouse_press_scroll_offset = 0; // scroll_offset from last mousePressEvent() call
	void setScrollOffset(int value);
};

} // namespace bWidgets
