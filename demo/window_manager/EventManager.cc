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

#include <list>

#include "bwRange.h"

#include "GPU.h"
#include "Stage.h"

#include "EventManager.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo {

EventManager& EventManager::ensureEventManager()
{
  static EventManager instance;
  return instance;
}

void EventManager::waitEvents()
{
  glfwWaitEvents();
}

bool EventManager::processEvents(WindowManager::WindowList& windows)
{
  for (Window& win : windows) {
    /* TODO, should register handler lists that are handled in
     * event-manager, for now, just doing it with callbacks */
    if (win.processEvents() == Window::WINDOW_ACTION_CLOSE) {
#if 0
			if (isMainWindow(win)) {
				return WM_ACTION_CLOSE;
			}
			else {
				removeWindow(win);
			}
#endif
      return false;
    }
  }

  return true;
}

void EventManager::setupWindowHandlers(Window& window)
{
  GLFWwindow& glfw_window = window.getGlfwWindow();

  glfwSetWindowUserPointer(&glfw_window, &window);
  glfwSetWindowSizeCallback(&glfw_window, handleWindowResizeEvent);
  glfwSetCursorPosCallback(&glfw_window, handleMouseMovementEvent);
  glfwSetMouseButtonCallback(&glfw_window, handleMouseButtonEvent);
  glfwSetScrollCallback(&glfw_window, handleMouseScrollEvent);
}

void EventManager::handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y)
{
  auto* win = (Window*)glfwGetWindowUserPointer(glfw_win);
  win->handleResizeEvent(new_win_x, new_win_y);
}

bwMouseButtonEvent::MouseButton EventManager::convertGlfwMouseButton(int glfw_button)
{
  switch (glfw_button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      return bwMouseButtonEvent::BUTTON_LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return bwMouseButtonEvent::BUTTON_RIGHT;
  }

  return bwMouseButtonEvent::BUTTON_UNKNOWN;
}

MouseEvent::MouseEventType EventManager::convertGlfwMouseButtonAction(int glfw_action)
{
  switch (glfw_action) {
    case GLFW_PRESS:
      return MouseEvent::MOUSE_EVENT_PRESS;
    case GLFW_RELEASE:
      return MouseEvent::MOUSE_EVENT_RELEASE;
  }

  return MouseEvent::MOUSE_EVENT_UNKNOWN;
}

void EventManager::handleMouseMovementEvent(GLFWwindow* glfw_win, double /*x*/, double /*y*/)
{
  const Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
  const bwPoint& position = win->getCursorPosition();
  MouseEvent event(MouseEvent::MOUSE_EVENT_MOVE, bwMouseButtonEvent::BUTTON_UNKNOWN, position);

  win->stage->handleMouseMovementEvent(event);
}

void EventManager::handleMouseButtonEvent(GLFWwindow* glfw_win,
                                          int glfw_button,
                                          int glfw_action,
                                          int /*glfw_mods*/)
{
  const Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
  const bwPoint& position = win->getCursorPosition();
  const MouseEvent::MouseEventType action_type = convertGlfwMouseButtonAction(glfw_action);
  const bwMouseButtonEvent::MouseButton mouse_button = convertGlfwMouseButton(glfw_button);
  MouseEvent event(action_type, mouse_button, position);

  win->stage->handleMouseButtonEvent(event);
}

void EventManager::handleMouseScrollEvent(GLFWwindow* glfw_win, double /*value_x*/, double value_y)
{
  if ((value_y > -1) && (value_y < 1)) {
    return;
  }

  const Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
  const MouseEvent::MouseEventType event_type = (value_y > 0) ?
                                                    MouseEvent::MOUSE_EVENT_SCROLL_UP :
                                                    MouseEvent::MOUSE_EVENT_SCROLL_DOWN;
  const bwPoint& position = win->getCursorPosition();
  MouseEvent event(event_type, bwMouseButtonEvent::BUTTON_WHEEL, position);
  bwMouseWheelEvent::Direction dir = (value_y > 0) ? bwMouseWheelEvent::Direction::UP :
                                                     bwMouseWheelEvent::Direction::DOWN;

  win->stage->handleMouseScrollEvent(event, dir);
}

}  // namespace bWidgetsDemo
