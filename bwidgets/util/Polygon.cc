#include "Polygon.h"
#include "Point.h"

using namespace bWidgets;

Polygon::Polygon() :
    vert_count(0)
{
}

Polygon::Polygon(const Polygon &poly) :
    vertices(poly.vertices), vert_count(poly.vert_count)
{
	
}

Polygon::Polygon(std::vector<Point> &vertices) :
    vertices(vertices), vert_count(vertices.size())
{
	
}

Polygon::Polygon(const unsigned int reserve_vertex_count)
{
	vertices.reserve(reserve_vertex_count);
}

void Polygon::addVertex(Point vertex)
{
	vertices.push_back(vertex);
	vert_count++;
}

void Polygon::addVertex(const float x, const float y)
{
	addVertex(Point(x, y));
}

void Polygon::reserve(const unsigned int count)
{
	vertices.reserve(count);
}

const std::vector<Point>& Polygon::getVertices() const
{
	return vertices;
}

Point& Polygon::operator[](const unsigned int index)
{
	return vertices[index];
}

bool Polygon::isDrawable() const
{
	return (vert_count > 0);
}
