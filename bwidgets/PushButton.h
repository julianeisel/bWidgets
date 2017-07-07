#pragma once

#include <string>

#include "Widget.h"


namespace bWidgets {

class PushButton : public Widget
{
public:
	PushButton(
	        const std::string& _text, int position_x = 0, int position_y = 0, int width = 10, int height = 10);

	void draw();
	void onClick();

private:
	const std::string text;
};

} // namespace bWidgets
