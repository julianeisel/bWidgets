#pragma once

#include <list>

#include "bwWidget.h"

#include "Event.h"

struct GLFWwindow;

namespace bWidgetsDemo {

class EventManager
{
public:
	// Constructor is private (singleton!)
	static EventManager& ensureEventManager();

	void waitEvents();
	bool processEvents(std::list<class Window*>& windows);

	static void setupWindowHandlers(class Window& window);

	bool isClickEvent();
	bool isDragging();

private:
	EventManager() {}
	~EventManager() {}
	EventManager(EventManager const&) = delete;

	void operator=(EventManager const&) = delete;

	static void handleWindowResizeEvent(
	        GLFWwindow* glfw_win,
	        int new_win_x, int new_win_y);
	static void handleMouseMovementEvent(
	        GLFWwindow* glfw_win,
	        double x, double y);
	static void handleMouseButtonEvent(
	        GLFWwindow* glfw_win,
	        int glfw_button,
	        int glfw_action,
	        int glfw_mods);
	static void handleMouseScrollEvent(
	        GLFWwindow* glfw_win,
	        double value_x, double value_y);
	static bWidgets::bwWidget::MouseButton convertGlfwMouseButton(
	        int glfw_button);
	static MouseEvent::MouseEventType convertGlfwMouseButtonAction(
	        int glfw_action);

	static bool is_dragging;
};

} // namespace bWidgetsDemo
