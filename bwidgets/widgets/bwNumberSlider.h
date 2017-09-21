#pragma once

#include "bwTextBox.h"

namespace bWidgets {

class bwNumberSlider : public bwTextBox
{
public:
	bwNumberSlider(
	        const unsigned int width_hint = 0, const unsigned int height_hint = 0);

	void draw(class bwStyle &style) const override;

	void mousePressEvent(const MouseButton button) override;
	void mouseReleaseEvent(const MouseButton button) override;
	void mouseClickEvent(const MouseButton button) override;
	void mouseDragEvent(const MouseButton button, const int drag_distance) override;

	void setValue(float value);
	float getValue() const;
	void setMinMax(float min, float max);

	void (*apply)(bwWidget& widget);

private:
	std::string valueToString(unsigned int precision) const;
	float calcValueIndicatorWidth(class bwStyle& style) const;

	union {
		struct {
			float value;
			float min, max;
			// Initial value before starting to edit.
			float initial_value;
			float precision;
		};
		// int value;
		// char value;
		// ...
	};
};

} // namespace bWidgets
