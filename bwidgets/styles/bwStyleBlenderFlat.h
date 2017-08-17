#pragma once

#include "bwStyle.h"

namespace bWidgets {

class bwStyleBlenderFlat : public bwStyle
{
public:
	bwStyleBlenderFlat();
	void setWidgetStyle(const class bwWidget& widget);

private:
	void setWidgetStyleDefault(const class bwWidget& widget);
	void setWidgetStyleRadioButton(const class bwWidget& widget);
	void setWidgetStyleLabel();
};

} // namespace bWidgets
