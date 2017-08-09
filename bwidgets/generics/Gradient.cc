#include <cassert>

#include "Point.h"

#include "Gradient.h"

using namespace bWidgets;


Color Gradient::calcPointColor(
        const Point& point, const Rectangle<unsigned int>& bounding_box) const
{
	const float pos_fac = (direction == Gradient::DIRECTION_TOP_BOTTOM) ?
	                          ((float)(point.y - bounding_box.ymin) / (float)bounding_box.height()) :
	                          ((float)(bounding_box.xmax - point.x) / (float)bounding_box.width());
	const float pos_fac_inv = 1.0f - pos_fac;

	assert((pos_fac >= 0.0f) && (pos_fac <= 1.0f));

	return Color((begin[0] * pos_fac) + (end[0] * pos_fac_inv),
	             (begin[1] * pos_fac) + (end[1] * pos_fac_inv),
	             (begin[2] * pos_fac) + (end[2] * pos_fac_inv),
	             (begin[3] * pos_fac) + (end[3] * pos_fac_inv));
}
