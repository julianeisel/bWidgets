#pragma once

#include "bwStyle.h"

namespace bWidgets {

class bwStyleFlat : public bwStyle
{
public:
	bwStyleFlat();

	void setWidgetStyle(class bwWidget& widget) override;
	void polish(bwWidget &) override
	{
		
	}
};

} // namespace bWidgets
