#pragma once

#include <string>

#include "Widget.h"


namespace bWidgets {

class PushButton : public Widget
{
public:
	PushButton(
	        const std::string& _text,
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw();

private:
	const std::string text;
};

} // namespace bWidgets
