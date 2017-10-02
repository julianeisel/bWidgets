#pragma once

#include <vector>

namespace bWidgets {

class bwPoint {
public:
	explicit bwPoint(float x = 0, float y = 0);

	bool operator==(const bwPoint& rhs) const;

	float x, y;
};

} // namepsace bWidgets
