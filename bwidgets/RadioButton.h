#pragma once

#include <string>

#include "Widget.h"

namespace bWidgets {

class RadioButton : public Widget
{
public:
	RadioButton(
	        const std::string& text,
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw(class Style &style);
	void onClick();

private:
	const std::string text;
};

} // bWidgets
