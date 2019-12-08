#pragma once

#include <vector>

namespace bWidgets {

class bwPoint {
 public:
  bwPoint(float x = 0, float y = 0);

  bool operator==(const bwPoint& rhs) const;

  float x, y;
};

bwPoint operator*(const bwPoint&, float);
bwPoint operator*(float, const bwPoint&);
bwPoint operator+(const bwPoint&, const bwPoint&);
bwPoint operator-(const bwPoint&, const bwPoint&);

float distance(bwPoint a, bwPoint b);

}  // namespace bWidgets
