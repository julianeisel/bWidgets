#include "bwPoint.h"

using namespace bWidgets;

bwPoint::bwPoint(float x, float y) :
    x(x), y(y)
{
	
}

bool bwPoint::operator==(const bwPoint& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}
