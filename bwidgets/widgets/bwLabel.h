#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"

namespace bWidgets {

class bwLabel : public bwWidget
{
public:
	bwLabel(
	        const std::string& text,
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

	void draw(class bwStyle& style) override;
	void registerProperties() override;

private:
	const std::string text;

	bwColor text_color;
};

} // namespace bWidgets
