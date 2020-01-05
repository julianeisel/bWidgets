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
  const bwPointVec& getVertices() const;

  bwPoint& operator[](const unsigned int index);

  bool isDrawable() const;

 protected:
  bwPointVec vertices{};
  unsigned long int vert_count{0};
};

}  // namespace bWidgets
