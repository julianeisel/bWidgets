#include "EventManager.h"
#include "GPU.h"
#include "Stage.h"

#include "Window.h"

using namespace bWidgetsDemo;


Window::Window(std::string name, unsigned int size_x, unsigned int size_y) :
    stage(NULL), width(size_x), height(size_y)
{
//	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

//	glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // For MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (width == 0) {
//		width = 0.8f * mode->width;
	}
	if (height == 0) {
//		height = 0.8 * mode->height;
	}

	glfw_window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
	glfwMakeContextCurrent(glfw_window);

	glEnable(GL_SCISSOR_TEST);

	GPU_init();

	EventManager::setupWindowHandlers(*this);
}

Window::~Window()
{
	if (stage) {
		delete stage;
	}
	glfwDestroyWindow(glfw_window);
}

void Window::draw()
{
	int width, height;

	glfwGetFramebufferSize(glfw_window, &width, &height);
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
	glClearColor(0.447f, 0.447f, 0.447f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (stage) {
		stage->draw();
	}

	glfwSwapBuffers(glfw_window);
}

Window::WindowAction Window::processEvents()
{
	if (glfwWindowShouldClose(glfw_window)) {
		return WINDOW_ACTION_CLOSE;
	}

	return WINDOW_ACTION_CONTINUE;
}

const bWidgets::bwPoint Window::getCursorPosition() const
{
	int win_size_y;
	double x, y;

	glfwGetCursorPos(glfw_window, &x, &y);
	glfwGetWindowSize(glfw_window, nullptr, &win_size_y);

	return bWidgets::bwPoint(x, win_size_y - y);
}

void Window::handleResizeEvent(const int new_win_x, const int new_win_y)
{
	width = new_win_x;
	height = new_win_y;
	stage->handleWindowResizeEvent(*this);
}

GLFWwindow *Window::getGlfwWindow() const
{
	return glfw_window;
}

int Window::getWidth() const
{
	return width;
}

int Window::getHeight() const
{
	return height;
}
