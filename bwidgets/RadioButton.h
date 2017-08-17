#pragma once

#include <string>

#include "Widget.h"

namespace bWidgets {

/**
 * \brief A radio-button like widget retaining a pressed state once pressed.
 *
 * \note: The way this works right now is not that nice. It's however made so it can work nicely with Blender.
 *
 * Normally, there's a group of radio buttons, once one of them is pressed, the others get un-pressed. It's an
 * exclusive behavior: There should only ever be one pressed radio button in the group. bWidgets doesn't
 * ensure this behavior at all as of now. The host application needs to do this.
 * In future, there should be a group-widget that ensures the exclusive behavior within bWidgets. We need to
 * make Blender ready for it first, though (or make it optional).
 */
class RadioButton : public Widget
{
public:
	RadioButton(
	        const std::string& text,
	        unsigned int position_x = 0, unsigned int position_y = 0,
	        unsigned int width = 10, unsigned int height = 10);

	void draw(class Style &style);
	void onClick();

	// XXX Not that pretty.
	void* custom_data;
	unsigned int valueID;

private:
	const std::string text;
};

} // bWidgets
