#pragma once

#include "Color.h"
#include "Rectangle.h"

namespace bWidgets {

/**
 * \brief Simple class for calculating colors for gradient fills.
 *
 * The implementation is currently pretty simple and limited, it's however all we need for
 * now. More features like unlimited stops at custom positions could be added if needed.
 */
struct Gradient
{
	enum Direction {
		DIRECTION_TOP_BOTTOM,
		DIRECTION_LEFT_RIGHT, // Unused and maybe not needed. Should work fine though.
	};

	Color begin, end;
	Direction direction;

	Color calcPointColor(
	        const class Point& point, const Rectangle<unsigned int>& bounding_box) const;
};

} // namespace bWidgets
