#pragma once

#include <vector>

namespace bWidgets {

class Polygon {
public:
	Polygon();
	Polygon(std::vector<class Point>& vertices);

	void addVertex(class Point& vertex);
	const std::vector<Point>& getVertices() const;

private:
	std::vector<class Point>* vertices;
};

} // namespace bWidgets
