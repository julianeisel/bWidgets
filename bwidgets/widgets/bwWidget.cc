#include "bwPoint.h"
#include "bwWidget.h"

using namespace bWidgets;


bwWidget::bwWidget(const WidgetType type, const bwRectanglePixel& rectangle) :
    type(type), state(STATE_NORMAL), rectangle(rectangle)
{
	
}

bool bwWidget::isCoordinateInside(const bwPoint& point) const
{
	return rectangle.isCoordinateInside(point.x, point.y);
}

void bwWidget::mousePressEvent(const MouseButton /*button*/)
{
	
}

void bwWidget::mouseReleaseEvent(const MouseButton /*button*/)
{
	
}

void bwWidget::mouseClickEvent(const MouseButton /*button*/)
{
	
}

void bwWidget::mouseDragEvent(const MouseButton /*button*/, const int /*drag_distance*/)
{
	
}

void bwWidget::mouseEnter()
{
	
}

void bwWidget::mouseLeave()
{
	
}
