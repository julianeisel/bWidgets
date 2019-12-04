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
#include "bwWidget.h"

namespace bWidgetsDemo {

class Event {
  friend class EventManager;

 protected:
  Event()
  {
  }
};

class MouseEvent : Event {
 public:
  enum MouseEventType {
    MOUSE_EVENT_PRESS,
    MOUSE_EVENT_RELEASE,
    MOUSE_EVENT_MOVE,
    MOUSE_EVENT_SCROLL_UP,
    MOUSE_EVENT_SCROLL_DOWN,

    MOUSE_EVENT_UNKNOWN
  };

  MouseEvent(MouseEventType type,
             bWidgets::bwWidget::MouseButton button,
             const bWidgets::bwPoint &location);

  bool isClick() const;

  bWidgets::bwWidget::MouseButton getButton() const;
  MouseEventType getType() const;
  bWidgets::bwDistance getDragDistance() const;

  static const bWidgets::bwPoint &getMouseLocation();

 private:
  MouseEventType type;
  bWidgets::bwWidget::MouseButton button;

  static bWidgets::bwPoint location;
  // Location during previous mouse button press.
  static bWidgets::bwPoint last_down_location;
  static bWidgets::bwWidget::MouseButton last_down_button;
};

}  // namespace bWidgetsDemo
