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

#include "bwDistance.h"
#include "bwEvent.h"
#include "bwWidget.h"

namespace bWidgetsDemo {

class Event {
  friend class EventManager;

 protected:
  Event() = default;
};

class MouseEvent : Event {
 public:
  enum class Type {
    PRESS,
    RELEASE,
    MOVE,
    SCROLL_UP,
    SCROLL_DOWN,

    UNKNOWN
  };

  using Button = bWidgets::bwMouseButtonEvent::Button;

  MouseEvent(Type type, Button button, const bWidgets::bwPoint& location);

  auto isClick() const -> bool;

  auto getButton() const -> Button;
  auto getType() const -> Type;

  static auto getMouseLocation() -> const bWidgets::bwPoint&;

 private:
  Type type;
  Button button;

  static bWidgets::bwPoint location;
  // Location during previous mouse button press.
  static bWidgets::bwPoint last_down_location;
  static Button last_down_button;
};

}  // namespace bWidgetsDemo
