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

#include <cassert>

#include "WindowManager.h"

#include "Application.h"

namespace bWidgetsDemo {

Application& Application::ensureApplication()
{
  static Application app;
  return app;
}

void Application::setup()
{
  WindowManager& wm = WindowManager::getWindowManager();
  wm.addWindow("bWidgets Demo");
}

void Application::mainLoop()
{
  WindowManager& wm = WindowManager::getWindowManager();
  wm.mainLoop();
}

void Application::exit()
{
}

}  // namespace bWidgetsDemo
