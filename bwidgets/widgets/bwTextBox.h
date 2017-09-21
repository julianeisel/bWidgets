#pragma once

#include "bwRectangle.h"
#include "bwWidget.h"

namespace bWidgets {

class bwTextBox : public bwWidget
{
public:
	bwTextBox(
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

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
