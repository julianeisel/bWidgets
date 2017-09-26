#pragma once

struct GLFWwindow;

namespace bWidgetsDemo {

class Window
{
public:
	Window(std::string name, unsigned int size_x = 500, unsigned int size_y = 300);
	~Window();

	enum WindowAction {
		WINDOW_ACTION_CONTINUE,
		WINDOW_ACTION_CLOSE,
	};
	WindowAction processEvents();
	void draw();

	const bWidgets::bwPoint getCursorPosition() const;
	void handleResizeEvent(const int new_win_x, const int new_win_y);

	class Stage* stage;

	GLFWwindow* getGlfwWindow() const;

	int getWidth() const;
	int getHeight() const;

private:
	GLFWwindow* glfw_window;
	unsigned int VertexArrayID;

	int width, height;
};

} // namespace bWidgetsDemo
