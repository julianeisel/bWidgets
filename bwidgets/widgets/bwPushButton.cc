#include <iostream>
#include <string>

#include "bwPainter.h"
#include "bwPolygon.h"
#include "bwStyle.h"

#include "bwPushButton.h"

using namespace bWidgets;


bwPushButton::bwPushButton(
        const std::string& text,
        unsigned int width_hint, unsigned int height_hint) :
    bwAbstractButton(text, WIDGET_TYPE_PUSH_BUTTON, width_hint, height_hint)
{
	
}
