#pragma once

#include "bwDistance.h"
#include "bwPoint.h"

namespace bWidgets {

class bwEvent
{
public:
	bwEvent(bwPoint _location) :
	    location(_location)
	{}

	// Where did the event happen?
	const bwPoint location;
	// TODO
//	void swallow();
};

class bwMouseButtonEvent : public bwEvent
{
public:
	enum MouseButton {
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_WHEEL,

		BUTTON_UNKNOWN
	};

	bwMouseButtonEvent(MouseButton _button, bwPoint _location) :
	    bwEvent(_location), button(_button)
	{}

	const MouseButton button;
};

class bwMouseButtonDragEvent : public bwMouseButtonEvent
{
public:
	bwMouseButtonDragEvent(MouseButton _button, bwPoint _location, bwDistance _drag_distance) :
	    bwMouseButtonEvent(_button, _location), drag_distance(_drag_distance)
	{}

	const bwDistance drag_distance;
};

} // namespace bWidgets
