#include <assert.h>
#include <iostream>

#include "EventHandler.h"
#include "Node.h"

using namespace bWidgets;
using namespace bWidgets::bwScreenGraph;


void EventHandler::addEventListener(
        EventHandler::EventType event_type,
        EventListener listener)
{
	assert(event_type < TOT_EVENT_TYPES);
	listeners[event_type].push_back(listener);
}
