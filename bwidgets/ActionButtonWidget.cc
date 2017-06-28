#include <iostream>
#include <string>

extern "C" {
#include "../extern/gawain/gawain/immediate.h"
}
#include "GLFW/glfw3.h"

#include "ShaderProgram.h"
#include "ActionButtonWidget.h"

using namespace bWidgets;


ActionButtonWidget::ActionButtonWidget(
        const std::string& _text, int position_x, int position_y, int _width, int _height) :
    text(_text)
{
	pos_x = position_x;
	pos_y = position_y;
	width = _width;
	height = _height;
}

void ActionButtonWidget::draw()
{
	ShaderProgram shader_program(ShaderProgram::SHADER_PROGRAM_ID_UNIFORM_COLOR);
	unsigned int pos = VertexFormat_add_attrib(immVertexFormat(), "pos", COMP_F32, 2, KEEP_FLOAT);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());
	immUniformColor3f(0.8f, 0.8f, 0.8f);

	immBegin(PRIM_TRIANGLE_FAN, 4);
	immVertex2f(pos, pos_x, pos_y);
	immVertex2f(pos, pos_x + width, pos_y);
	immVertex2f(pos, pos_x + width, pos_y + height);
	immVertex2f(pos, pos_x, pos_y + height);
	immEnd();
}

void ActionButtonWidget::onClick()
{
	std::cout << text << std::endl;
}
