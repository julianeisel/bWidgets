#include <iostream>
#include <vector>

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "GLFW/glfw3.h"
#include "../gpu/ShaderProgram.h"

// bWidgets lib
#include "Widget.h"
#include "Polygon.h"
#include "Point.h"

#include "GPU.h"

#include "Stage.h"

#include "Window.h"


using namespace bWidgetDemo;

/**
 * The main polygon draw callback which is used to draw all geometry of widgets.
 */
static void stage_polygon_draw_cb(bWidgets::Polygon& poly)
{
	ShaderProgram shader_program(ShaderProgram::SHADER_PROGRAM_ID_UNIFORM_COLOR);
	unsigned int pos = VertexFormat_add_attrib(immVertexFormat(), "pos", COMP_F32, 2, KEEP_FLOAT);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());
	immUniformColor3f(0.8f, 0.8f, 0.8f);

	immBegin(PRIM_TRIANGLE_FAN, 4);
	for (const bWidgets::Point& vertex : poly.getVertices()) {
		immVertex2f(pos, vertex.x, vertex.y);
	}
	immEnd();
}

Stage::Stage()
{
	// Initialize drawing callbacks for the stage
	bWidgets::Polygon::draw = stage_polygon_draw_cb;
}

Stage::~Stage()
{
	for (bWidgets::Widget* widget : widgets) {
		delete widget;
	}
}

void Stage::draw(unsigned int width, unsigned int height)
{
	gpuOrtho(0.0f, width, 0.0f, height);

	for (bWidgets::Widget* widget : widgets) {
		widget->draw();
	}
}

void Stage::widgetAdd(bWidgets::Widget* widget)
{
	widgets.push_front(widget);
}

void Stage::handleMouseButtonEvent(Window* win, const int mouse_xy[2], int button, int action, int mods)
{
	for (bWidgets::Widget* widget : widgets) {
		if (widget->isCoordinateInside(mouse_xy[0], mouse_xy[1])) {
			widget->onClick();
		}
	}
}
