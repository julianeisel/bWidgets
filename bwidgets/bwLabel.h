#pragma once

#include <string>

#include "bwWidget.h"

namespace bWidgets {

class bwLabel : public bwWidget
{
public:
	bwLabel(
	        const std::string& text,
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw(class bwStyle& style);

private:
	const std::string text;
};

} // namespace bWidgets
