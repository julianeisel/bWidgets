/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <iostream>

#include "GPU.h"

#include "EventManager.h"
#include "WindowManager.h"

using namespace bWidgetsDemo;


static void wm_glfw_error_callback(int /* error */, const char* description)
{
	std::cout << "Error: " << description << std::endl;
}

WindowManager::WindowManager() :
    event_manager(EventManager::ensureEventManager())
{
	if (!glfwInit()) {
		assert(0);
	}
	glfwSetErrorCallback(wm_glfw_error_callback);

//	GPU_init(); // needs context, so delay until window creation
}

WindowManager& WindowManager::CreateWindowManager()
{
	static WindowManager instance;
	return instance;
}

WindowManager::~WindowManager()
{
	GPU_exit();
	for (Window *win : windows) {
		delete win;
	}
	glfwTerminate();
}

WindowManager::WindowManagerAction WindowManager::processEvents()
{
	event_manager.waitEvents();
	if (!event_manager.processEvents(windows)) {
		return WM_ACTION_CLOSE;
	}

	return WM_ACTION_CONTINUE;
}

void WindowManager::drawWindows()
{
	for (Window* win: windows) {
		win->draw();
	}
}

void WindowManager::mainLoop()
{
	while (processEvents() == WM_ACTION_CONTINUE) {
		drawWindows();
	}
}

Window* WindowManager::addWindow(std::string name)
{
	Window *win_new = new Window(name);

	windows.push_back(win_new);
	if (windows.size() == 1) {
		main_win = win_new;
	}

	return win_new;
}

void WindowManager::removeWindow(Window* win)
{
	windows.remove(win);
}

const bool WindowManager::isMainWindow(const Window *win) const
{
	return win == main_win;
}
