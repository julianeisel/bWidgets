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

	static void handleMouseButtonEvent(GLFWwindow *glfw_win, int button, int action, int mods);

private:
	EventManager() {};
	~EventManager() {};
	EventManager(EventManager const&) = delete;
	void operator=(EventManager const&) = delete;
};

} // namespace bWidgetDemo
