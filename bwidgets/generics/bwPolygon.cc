#include "bwPolygon.h"
#include "bwPoint.h"

using namespace bWidgets;

bwPolygon::bwPolygon() :
    vert_count(0)
{
}

bwPolygon::bwPolygon(const bwPolygon &poly) :
    vertices(poly.vertices), vert_count(poly.vert_count)
{
	
}

bwPolygon::bwPolygon(bwPointVec& vertices) :
    vertices(vertices), vert_count(vertices.size())
{
	
}

bwPolygon::bwPolygon(const unsigned int reserve_vertex_count)
{
	vertices.reserve(reserve_vertex_count);
}

void bwPolygon::addVertex(bwPoint vertex)
{
	vertices.push_back(vertex);
	vert_count++;
}

void bwPolygon::addVertex(const float x, const float y)
{
	addVertex(bwPoint(x, y));
}

void bwPolygon::reserve(const unsigned int count)
{
	vertices.reserve(count);
}

const bwPointVec& bwPolygon::getVertices() const
{
	return vertices;
}

bwPoint& bwPolygon::operator[](const unsigned int index)
{
	return vertices[index];
}

bool bwPolygon::isDrawable() const
{
	return (vert_count > 0);
}
