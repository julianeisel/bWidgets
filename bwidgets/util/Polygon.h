#pragma once

#include <vector>

namespace bWidgets {

class Polygon {
public:
	Polygon();
	Polygon(std::vector<class Point>& vertices);

	void addVertex(class Point& vertex);
	void draw();

private:
	std::vector<class Point>* vertices;
};

} // namespace bWidgets
