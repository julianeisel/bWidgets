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

#pragma once

#include "bwWidget.h"

#include "WindowManager.h"

#include "Event.h"

struct GLFWwindow;

namespace bWidgetsDemo {

class EventManager {
 public:
  // Constructor is private (singleton!)
  static auto ensureEventManager() -> EventManager&;
  static void setupWindowHandlers(Window& window);

  void waitEvents();
  auto processEvents(WindowManager::WindowList& windows) -> bool;

  auto isClickEvent() -> bool;

 private:
  EventManager() = default;
  ~EventManager() = default;
  EventManager(EventManager const&) = delete;

  void operator=(EventManager const&) = delete;

  static void handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y);
  static void handleWindowContentScaleEvent(GLFWwindow* glfw_win,
                                            float new_scale_x,
                                            float new_scale_y);
  static void handleMouseMovementEvent(GLFWwindow* glfw_win, double x, double y);
  static void handleMouseButtonEvent(GLFWwindow* glfw_win,
                                     int glfw_button,
                                     int glfw_action,
                                     int glfw_mods);
  static void handleMouseScrollEvent(GLFWwindow* glfw_win, double value_x, double value_y);
  static auto convertGlfwMouseButton(int glfw_button) -> bWidgets::bwMouseButtonEvent::Button;
  static auto convertGlfwMouseButtonAction(int glfw_action) -> MouseEvent::Type;
};

}  // namespace bWidgetsDemo
