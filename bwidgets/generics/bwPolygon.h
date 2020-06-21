#pragma once

#include <vector>

namespace bWidgets {

/* TODO for (2D-)polygons, we should actually use ints, not floats. Prevents precision and rounding
 * errors. */
using bwPointVec = std::vector<class bwPoint>;

class bwPolygon {
 public:
  bwPolygon() = default;
  bwPolygon(const bwPolygon& poly) = default;
  bwPolygon(const bwPointVec& vertices);
  explicit bwPolygon(const unsigned int reserve_vertex_count);

  void addVertex(class bwPoint vertex);
  void addVertex(const float x, const float y);
  void addVertex(const int x, const int y);
  void reserve(const unsigned int count);
  auto getVertices() const -> const bwPointVec&;

  auto operator[](const unsigned int index) -> bwPoint&;

  auto isDrawable() const -> bool;

 protected:
  bwPointVec vertices{};
  size_t vert_count{0};
};

}  // namespace bWidgets
