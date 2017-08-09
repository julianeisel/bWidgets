#pragma once

#include <vector>

namespace bWidgets {

class Polygon
{
public:
	Polygon();
	Polygon(const Polygon& poly);
	Polygon(std::vector<class Point>& vertices);
	explicit Polygon(const unsigned int reserve_vertex_count);

	void addVertex(class Point vertex);
	void addVertex(const float x, const float y);
	void reserve(const unsigned int count);
	const std::vector<Point>& getVertices() const;

	Point& operator[](const unsigned int index);

	bool isDrawable() const;

protected:
	std::vector<class Point> vertices;
	unsigned int vert_count;
};

} // namespace bWidgets
