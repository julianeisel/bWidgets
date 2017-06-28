#pragma once

#include <list>
#include <memory>
#include "Window.h"


namespace bWidgetDemo {

class WindowManager
{
public:
	// Constructor is private (singleton!)
	static WindowManager& CreateWindowManager();

	void mainLoop();
	Window* addWindow(std::string name);
	const bool isMainWindow(const Window *win) const;

	void removeWindow(Window *win);

private:
	WindowManager();
	~WindowManager();
	WindowManager(WindowManager const&) = delete;
	void operator=(WindowManager const&) = delete;

	enum WindowManagerAction {
		WM_ACTION_CONTINUE,
		WM_ACTION_CLOSE,
	};
	WindowManagerAction processEvents();
	void drawWindows();

	class EventManager& event_manager;
	std::list<Window*> windows;
	Window* main_win;
};

} // namespace bWidgetDemo
