#pragma once

#include <vector>

namespace bWidgets {

class bwPoint
{
public:
	bwPoint(float x = 0, float y = 0);

	bool operator==(const bwPoint& rhs) const;

	float x, y;
};


// Getting linker errors when putting this into bwPoint.cc... hrmpf..
inline bwPoint operator*(const bwPoint& lhs, float factor)
{
	return bwPoint(lhs.x * factor, lhs.y * factor);
}
inline bwPoint operator*(float factor, const bwPoint& rhs)
{
	return bwPoint(rhs.x * factor, rhs.y * factor);
}
inline bwPoint operator+(const bwPoint& lhs, const bwPoint& rhs)
{
	return bwPoint(lhs.x + rhs.x, lhs.y + rhs.y);
}

} // namepsace bWidgets
