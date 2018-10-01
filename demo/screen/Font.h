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

#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Pixmap.h"

#include "bwColor.h"
#include "bwRectangle.h"
#include "bwUtil.h"


namespace bWidgetsDemo {

class FontGlyph;
class Pen;

class Font {
public:
	enum AntiAliasingMode {
		/** Default, pixel coverage based AA. The alpha value of a pixel is
		 * determined by how much it overlaps with filled the glyph outline. */
		NORMAL_COVERAGE,
		/* Works similar to NORMAL_COVERAGE, but gives up to 3x the horizontal
		 * resolution by addressing RGB channels separately rather than the
		 * entire pixel. A filter is used to minimize resulting color fringes,
		 * making them invisible to most people. */
		SUBPIXEL_LCD_RGB_COVERAGE,
	};

	~Font();

	static void initFontReading();
	static Font* loadFont(const std::string& name, const std::string& path);

	void render(const std::string& text, const int pos_x, const int pos_y);
	unsigned int calculateStringWidth(const std::string &text);

	void setFontAntiAliasingMode(AntiAliasingMode);
	void setHinting(bool value);
	void setSize(const float size);
	int getSize() const;

	const bWidgets::bwColor& getActiveColor() const;
	void setActiveColor(const bWidgets::bwColor &value);

	const bWidgets::bwRectanglePixel& getMask() const;
	void setMask(const bWidgets::bwRectanglePixel& value);

private:
	Font() = default;

	void renderGlyph(
	        const FontGlyph& glyph, const FontGlyph* previous_glyph,
	        const unsigned int attr_pos, const unsigned int attr_texcoord,
	        Pen& pen) const;

	float getKerningDistance(const FontGlyph& left, const FontGlyph& right) const;
	/* Accesses private members, so make it a member function. Would be better
	 * to keep freetype specific stuff out of the general Font class, but
	 * ignoring for now since this is just the demo app anyway. */
	FT_Int32 getFreeTypeLoadFlags();

	// The freetype library handle.
	static FT_Library ft_library;
	// The freetype font handle.
	FT_Face face;
	// Was font type (face) changed since last draw?
	static bool changed;

	// Height in pixels.
	int size{0};

	bWidgets::bwColor active_color;
	bWidgets::bwRectanglePixel mask;
	AntiAliasingMode render_mode;
	bool use_hinting;


	class FontGlyphCache {
	// Everything public, this nested class is private to Font anyway.
	public:
		void invalidate();
		void ensureUpdated(Font&);
		const FontGlyph& getCachedGlyph(const Font&, const unsigned char) const;

		bool is_dirty{true};
		std::vector<std::unique_ptr<FontGlyph>> cached_glyphs;
	private:
		void loadGlyphsIntoCache(Font&);
	};

	FontGlyphCache cache;
};

class FontGlyph {
public:
	FontGlyph(
	        const unsigned int index,
	        bWidgets::bwPtr<Pixmap>&& pixmap,
	        const int offset_left, const int offset_top,
	        const int advance_width);
	FontGlyph();

	bool is_valid;

	unsigned int index; // Same as freetype index

	bWidgets::bwPtr<Pixmap> pixmap;
	int offset_left, offset_top; // bitmap_left, bitmap_top
	int advance_width; // advance.x
	int pitch;
};

} // namespace bWidgetsDemo
