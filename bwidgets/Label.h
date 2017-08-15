#pragma once

#include <string>

#include "Widget.h"

namespace bWidgets {

class Label : public Widget
{
public:
	Label(
	        const std::string& text,
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw(class Style& style);
	void onClick();
	void mouseEnter();
	void mouseLeave();

private:
	const std::string text;
};

} // namespace bWidgets
