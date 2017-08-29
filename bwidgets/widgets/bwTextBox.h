#pragma once

#include "bwRectangle.h"
#include "bwWidget.h"

namespace bWidgets {

class bwTextBox : public bwWidget
{
public:
	bwTextBox(
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw(class bwStyle &style) const override;
	void onClick(const MouseButton mouse_button) override;
	void mouseEnter() override;
	void mouseLeave() override;

	void setText(const std::string& value);
	const std::string& getText() const;

	bwRectanglePixel selection_rectangle;

private:
	std::string text;
};

} // namespace bWidgets
