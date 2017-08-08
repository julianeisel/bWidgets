#pragma once

#include <list>

namespace bWidgetDemo {

class EventManager
{
public:
	// Constructor is private (singleton!)
	static EventManager& CreateEventManager();

	void waitEvents();
	bool processEvents(std::list<class Window*>& windows);

	static void setupWindowHandlers(class Window& window);

private:
	EventManager() {}
	~EventManager() {}
	EventManager(EventManager const&) = delete;
	void operator=(EventManager const&) = delete;

	static void handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y);
	static void handleMouseMovementEvent(GLFWwindow* glfw_win, double x, double y);
	static void handleMouseButtonEvent(GLFWwindow* glfw_win, int button, int action, int mods);
};

} // namespace bWidgetDemo
