#pragma once

#include <vector>

namespace bWidgets {

class bwPoint {
 public:
  bwPoint(float x = 0, float y = 0);

  auto operator==(const bwPoint& rhs) const -> bool;

  float x, y;
};

auto operator*(const bwPoint&, float) -> bwPoint;
auto operator*(float, const bwPoint&) -> bwPoint;
auto operator+(const bwPoint&, const bwPoint&) -> bwPoint;
auto operator-(const bwPoint&, const bwPoint&) -> bwPoint;

auto distance(bwPoint a, bwPoint b) -> float;

}  // namespace bWidgets
