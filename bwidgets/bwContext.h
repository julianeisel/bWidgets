#pragma once

namespace bWidgets {

class bwWidget;

/**
 * \brief Important state information for the screen-graph.
 *
 * Blender used to manage special states of widgets (e.g. hovered, active,
 * etc.) mostly using per widget flags. To query these special widgets (i.e.
 * the currently hovered widget), a lookup had to be performed.
 * Rather than requiring lookups, bWidgets uses this context data-struture for
 * access to special state widgets.
 */
struct bwContext
{
};

} // namespace bWidgets
