extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "GLFW/glfw3.h"
#include "../ShaderProgram.h"

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

void Polygon::draw()
{
	ShaderProgram shader_program(ShaderProgram::SHADER_PROGRAM_ID_UNIFORM_COLOR);
	unsigned int pos = VertexFormat_add_attrib(immVertexFormat(), "pos", COMP_F32, 2, KEEP_FLOAT);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());
	immUniformColor3f(0.8f, 0.8f, 0.8f);

	immBegin(PRIM_TRIANGLE_FAN, 4);
	for (Point& vertex: *vertices) {
		immVertex2f(pos, vertex.x, vertex.y);
	}
	immEnd();
}
