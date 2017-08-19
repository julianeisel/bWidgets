#include "bwWidget.h"

using namespace bWidgets;


bwWidget::bwWidget(const WidgetType type, const bwRectanglePixel& rectangle) :
    type(type), rectangle(rectangle)
{
	
}

bool bwWidget::isCoordinateInside(int x, int y)
{
	return rectangle.isCoordinateInside(x, y);
}

void bwWidget::onClick(const MouseButton /*button*/)
{
	
}

void bwWidget::mouseEnter()
{
	
}

void bwWidget::mouseLeave()
{
	
}
