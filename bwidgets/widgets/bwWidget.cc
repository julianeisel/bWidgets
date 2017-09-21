#include "bwPoint.h"
#include "bwWidget.h"

using namespace bWidgets;


bwWidget::bwWidget(
        const WidgetType type,
        const unsigned int width_hint, const unsigned int height_hint) :
    type(type),
    state(STATE_NORMAL),
    rectangle(0, 0, 0, 0),
    width_hint(width_hint), height_hint(height_hint)
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
