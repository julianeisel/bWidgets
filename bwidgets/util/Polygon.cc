#include "Polygon.h"
#include "Point.h"

using namespace bWidgets;

Polygon::Polygon()
{
}

Polygon::Polygon(std::vector<Point> &vertices) :
    vertices(&vertices)
{
	
}

void Polygon::addVertex(Point& vertex)
{
	vertices->push_back(vertex);
}

const std::vector<Point>& Polygon::getVertices() const
{
	return *vertices;
}
