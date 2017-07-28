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
	void handleResizeEvent(const int new_win_x, const int new_win_y);

	class Stage* stage;

	class GLFWwindow *getGlfwWindow() const;

	int getWidth() const;
	int getHeight() const;

private:
	class GLFWwindow* glfw_window;
	unsigned int VertexArrayID;

	int width, height;
};

} // namespace bWidgetDemo
