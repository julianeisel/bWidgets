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

#include "glm.hpp"

#include "EventManager.h"

namespace bWidgetsDemo {

bWidgets::bwPoint MouseEvent::location{};
bWidgets::bwPoint MouseEvent::last_down_location{};
bWidgets::bwMouseButtonEvent::MouseButton MouseEvent::last_down_button =
    bWidgets::bwMouseButtonEvent::BUTTON_UNKNOWN;

MouseEvent::MouseEvent(MouseEventType type,
                       bWidgets::bwMouseButtonEvent::MouseButton _button,
                       const bWidgets::bwPoint& _location)
    : type(type)
{
  if (type == MOUSE_EVENT_PRESS) {
    last_down_location = _location;
    last_down_button = _button;
  }
  else if (type == MOUSE_EVENT_MOVE) {
    _button = last_down_button;
  }
  location = _location;
  button = _button;
}

bool MouseEvent::isClick() const
{
  if ((type == MOUSE_EVENT_RELEASE) && (button == last_down_button)) {
    const float dist = glm::distance(glm::vec2(location.x, location.y),
                                     glm::vec2(last_down_location.x, last_down_location.y));
    if (dist <= 3) {
      return true;
    }
  }

  return false;
}

bWidgets::bwMouseButtonEvent::MouseButton MouseEvent::getButton() const
{
  return button;
}

MouseEvent::MouseEventType MouseEvent::getType() const
{
  return type;
}

const bWidgets::bwPoint& MouseEvent::getMouseLocation()
{
  return location;
}

}  // namespace bWidgetsDemo
