#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"


namespace bWidgets {

class bwLabel : public bwWidget
{
public:
	bwLabel(
	        std::string text,
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

	void draw(class bwStyle& style) override;
	void registerProperties() override;
	const std::string* getLabel() const override;

	bwLabel& setIcon(const class bwIconInterface& icon_interface);

private:
	const std::string text;
	const class bwIconInterface* icon{nullptr};

	bwColor text_color;
};

} // namespace bWidgets
