#include <iostream>
#include <string>

#include "GPU.h"

#include "Stage.h"

#include "EventManager.h"
#include "Window.h"

using namespace bWidgetDemo;


Window::Window(std::string name, float size_x, float size_y) :
    stage(NULL)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // For MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (size_x == 0) {
		size_x = 0.8f * mode->width;
	}
	if (size_y == 0) {
		size_y = 0.8 * mode->height;
	}

	glfw_window = glfwCreateWindow(size_x, size_y, name.data(), NULL, NULL);
	glfwMakeContextCurrent(glfw_window);

	GPU_init();

	glfwSetWindowUserPointer(glfw_window, this);
	glfwSetMouseButtonCallback(glfw_window, EventManager::handleMouseButtonEvent);
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
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (stage) {
		stage->draw(width, height);
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

void Window::getCursorPosition(int r_position[2])
{
	int win_size_y;
	double x, y;

	glfwGetCursorPos(glfw_window, &x, &y);
	glfwGetWindowSize(glfw_window, nullptr, &win_size_y);

	r_position[0] = (int)x;
	r_position[1] = win_size_y - (int)y;
}
