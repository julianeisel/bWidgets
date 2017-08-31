#pragma once

#include "bwTextBox.h"

namespace bWidgets {

class bwNumberSlider : public bwTextBox
{
public:
	bwNumberSlider(
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw(class bwStyle &style) const override;

	void mousePressEvent(const MouseButton button) override;
	void mouseReleaseEvent(const MouseButton button) override;
	void mouseClickEvent(const MouseButton button) override;
	void mouseDragEvent(const MouseButton button, const int drag_distance) override;

	void setValue(float value);
	void setMinMax(float min, float max);

private:
	std::string valueToString(unsigned int precision) const;
	float calcValueIndicatorWidth(class bwStyle& style) const;

	union {
		struct {
			float value;
			float min, max;
			// Initial value before starting to edit.
			float initial_value;
		};
		// int value;
		// char value;
		// ...
	};
};

} // namespace bWidgets
