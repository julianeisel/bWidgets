#include <iostream>
#include <string>

#include "bwPainter.h"
#include "bwPolygon.h"
#include "bwStyle.h"

#include "bwPushButton.h"

using namespace bWidgets;


bwPushButton::bwPushButton(
        const std::string& text,
        unsigned int position_x, unsigned int position_y,
        unsigned int _width, unsigned int _height) :
    bwAbstractButton(
        text, WIDGET_TYPE_PUSH_BUTTON,
        bwRectanglePixel(position_x, position_x + _width, position_y, position_y + _height))
{
	
}
