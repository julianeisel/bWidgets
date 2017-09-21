#pragma once

#include <string>

#include "bwAbstractButton.h"


namespace bWidgets {

class bwPushButton : public bwAbstractButton
{
public:
	bwPushButton(
	        const std::string& text,
	        unsigned int width_hint = 0, unsigned int height_hint = 0);
};

} // namespace bWidgets
