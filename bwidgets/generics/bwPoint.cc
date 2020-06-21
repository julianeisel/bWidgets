#include <cmath>

#include "bwPoint.h"

namespace bWidgets {

bwPoint::bwPoint(float x, float y) : x(x), y(y)
{
}

auto bwPoint::operator==(const bwPoint& rhs) const -> bool
{
  return (x == rhs.x) && (y == rhs.y);
}

auto operator*(const bwPoint& lhs, float factor) -> bwPoint
{
  return {lhs.x * factor, lhs.y * factor};
}
auto operator*(float factor, const bwPoint& rhs) -> bwPoint
{
  return {rhs.x * factor, rhs.y * factor};
}
auto operator+(const bwPoint& lhs, const bwPoint& rhs) -> bwPoint
{
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}
auto operator-(const bwPoint& lhs, const bwPoint& rhs) -> bwPoint
{
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

auto distance(bwPoint a, bwPoint b) -> float
{
  return std::sqrt(std::pow(a.x - b.x, 2.0f) + std::pow(a.y - b.y, 2.0f));
}

}  // namespace bWidgets
