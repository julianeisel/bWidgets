#pragma once

#include "Style.h"

namespace bWidgets {

class StyleBlenderFlat : public Style
{
public:
	StyleBlenderFlat();
	void setWidgetStyle(const class Widget& widget);

private:
	void setWidgetStyleDefault(const class Widget& widget);
	void setWidgetStyleLabel();
	void setWidgetStyleRadioButton(const class Widget& widget);
};

} // namespace bWidgets
