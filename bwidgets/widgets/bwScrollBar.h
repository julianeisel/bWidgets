#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwScrollBar : public bwAbstractButton
{
public:
	bwScrollBar(
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

	void draw(bwStyle &style) override;
	void onMousePress(bwMouseButtonEvent&) override;
	void onMouseRelease(bwMouseButtonEvent&) override;
	void onMouseDrag(bwMouseButtonDragEvent&) override;

	float ratio = 0.0f; // Ration between content and area height (max 1.0f).
	int scroll_offset = 0;

private:
	int mouse_press_scroll_offset = 0; // scroll_offset from last onMousePress() call
	void setScrollOffset(int value);
};

} // namespace bWidgets
