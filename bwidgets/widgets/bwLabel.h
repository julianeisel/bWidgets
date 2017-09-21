#pragma once

#include <string>

#include "bwWidget.h"

namespace bWidgets {

class bwLabel : public bwWidget
{
public:
	bwLabel(
	        const std::string& text,
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

	void draw(class bwStyle& style) const override;

private:
	const std::string text;
};

} // namespace bWidgets
