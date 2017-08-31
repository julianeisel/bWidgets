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
	void mousePressEvent(const MouseButton button) override;
	void mouseEnter() override;
	void mouseLeave() override;

	void startTextEditing();
	void endTextEditing();

	void setText(const std::string& value);
	const std::string& getText() const;

	bwRectanglePixel selection_rectangle;

protected:
	std::string text;
	bool is_text_editing;
	bool is_dragging;
};

} // namespace bWidgets
