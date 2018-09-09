#pragma once

#include "bwColor.h"
#include "bwRectangle.h"

namespace bWidgets {

/**
 * \brief Simple class for calculating colors for gradient fills.
 *
 * The implementation is currently pretty simple and limited, it's however all we need for
 * now. More features like unlimited stops at custom positions could be added if needed.
 */
class bwGradient
{
public:
	enum Direction {
		DIRECTION_TOP_BOTTOM,
		DIRECTION_LEFT_RIGHT,
	};

	bwGradient() = default;
	explicit bwGradient(
	        const bwColor& base_color,
	        float shade_begin, float shade_end,
	        Direction direction = DIRECTION_TOP_BOTTOM);

	bwColor calcPointColor(
	        const class bwPoint& point,
	        const bwRectanglePixel& bounding_box) const;

	bwColor begin, end;
	Direction direction{DIRECTION_TOP_BOTTOM};
};

} // namespace bWidgets
