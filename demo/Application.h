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

namespace bWidgetsDemo {

/**
 * Singleton class for general data of the bWidget Demo Application.
 */
class Application {
 public:
  static Application &ensureApplication();

  void setup();
  void mainLoop();
  void exit();

  static float interfaceScale();

 private:
  Application()
  {
  }
  // Ensure uniqueness
  Application(Application const &) = delete;
  void operator=(Application const &) = delete;
};

}  // namespace bWidgetsDemo
