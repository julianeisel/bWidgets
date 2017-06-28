#pragma once

#include <string>


namespace bWidgetDemo {

class Window
{
public:
	Window(std::string name, float size_x = 0, float size_y = 0);
	~Window();

	enum WindowAction {
		WINDOW_ACTION_CONTINUE,
		WINDOW_ACTION_CLOSE,
	};
	WindowAction processEvents();
	void draw();

	void getCursorPosition(int r_position[2]);

	class Stage* stage;

private:
	class GLFWwindow* glfw_window;
	unsigned int VertexArrayID;
};

} // namespace bWidgetDemo
