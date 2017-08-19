#pragma once

#include "bwStyle.h"

namespace bWidgets {

class bwStyleBlenderClassic : public bwStyle
{
public:
	bwStyleBlenderClassic();

	void setWidgetStyle(const class bwWidget& widget);

private:
	void setWidgetStyleDefault(const class bwWidget& widget);
	void setWidgetStyleRadioButton(const class bwWidget& widget);
	void setWidgetStyleLabel();
	void setWidgetStyleTextBox(const class bwWidget& widget);
};

} // namespace bWidgets
