#pragma once

#include <string>

#include "bwAbstractButton.h"


namespace bWidgets {

class bwPushButton : public bwAbstractButton
{
public:
	bwPushButton(
	        const std::string& text,
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);
};

} // namespace bWidgets
