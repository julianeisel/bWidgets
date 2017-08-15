#pragma once

#include "Style.h"

namespace bWidgets {

struct StyleBlenderClassic : Style
{
	void setWidgetStyle(const class Widget& widget);

private:
	void setWidgetStyleDefault(const class Widget& widget);
	void setWidgetStyleLabel();
	void setWidgetStyleRadioButton(const class Widget& widget);
};

} // namespace bWidgets
