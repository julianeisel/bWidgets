#include <assert.h>

#include "EventHandler.h"

using namespace bWidgets::bwScreenGraph;


void EventHandler::addEventListener(
        EventHandler::EventType event_type,
        EventListener listener)
{
	assert(event_type < TOT_EVENT_TYPES);
	listeners[event_type].push_back(listener);
}
