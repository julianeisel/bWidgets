#pragma once

#include <vector>

namespace bWidgets {

class bwPolygon
{
public:
	bwPolygon();
	bwPolygon(const bwPolygon& poly);
	bwPolygon(std::vector<class bwPoint>& vertices);
	explicit bwPolygon(const unsigned int reserve_vertex_count);

	void addVertex(class bwPoint vertex);
	void addVertex(const float x, const float y);
	void reserve(const unsigned int count);
	const std::vector<bwPoint>& getVertices() const;

	bwPoint& operator[](const unsigned int index);

	bool isDrawable() const;

protected:
	std::vector<class bwPoint> vertices;
	unsigned int vert_count;
};

} // namespace bWidgets
