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

#include <list>
#include <memory>

#include "bwScrollBar.h"
#include "bwStyle.h"


namespace bWidgetsDemo {

class Stage
{
	friend class ScrollbarApplyValueFunctor;
	friend class StyleSetter;
	friend class UseCSSVersionToggleSetter;

public:
	Stage(const unsigned int mask_width, const unsigned int mask_height);
	virtual ~Stage() = default;

	void draw();

	void handleMouseMovementEvent(
	        const class MouseEvent& event);
	void handleMouseButtonEvent(
	        const class MouseEvent& event);
	void handleMouseDragEvent(
	        const class MouseEvent& event);
	void handleMouseScrollEvent(
	        const class MouseEvent& event);
	void handleWindowResizeEvent(
	        const class Window& win);

	static void setInterfaceScale(const float value);
	static void setFontHinting(const bool value);

protected:
	bWidgets::bwPointer<class RootLayout> layout;

	// Static members, global UI data for all stages
	static bWidgets::bwPointer<bWidgets::bwStyle> style;
	static bWidgets::bwPointer<class Font> font;
	static bWidgets::bwPointer<class IconMap> icon_map;
	static bWidgets::bwPointer<class StyleSheet> style_sheet;
	static float interface_scale;

	unsigned int mask_width, mask_height;
	int vert_scroll = 0;

	virtual void activateStyleID(bWidgets::bwStyle::StyleTypeID type_id);

private:
	// Not part of the layout yet. We'd need to support horizontal root layouts first.
	bWidgets::bwPointer<bWidgets::bwScrollBar> scrollbar;
	// The lastly hovered widget. Stored to detect mouse-leave events without lookups.
	bWidgets::bwOptional<bWidgets::bwWidget*> last_hovered;
	bWidgets::bwOptional<bWidgets::bwWidget*> dragged_widget;

	static void StyleSheetPolish(bWidgets::bwWidget& widget);

	void initFonts();
	void initIcons();
	void setStyleSheet(const std::string& filepath);
	void drawScrollbars();
	void updateContentBounds();
	void validizeScrollValue();
	bWidgets::bwOptional<std::reference_wrapper<bWidgets::bwWidget>> findWidgetAt(const bWidgets::bwPoint& coordinate);
	void updateWidgetHovering(
	        const MouseEvent&,
	        bWidgets::bwWidget&);
	bool handleWidgetMouseButtonEvent(
	        const MouseEvent&,
	        bWidgets::bwWidget&);

	bool isScrollable() const;
	unsigned int getScrollbarWidth() const;
	unsigned int getContentWidth() const;
	unsigned int getContentHeight() const;
	void setScrollValue(int value);
};

} // namespace bWidgetsDemo
