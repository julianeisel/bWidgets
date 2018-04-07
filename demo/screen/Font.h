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

#include "bwColor.h"
#include "bwRectangle.h"


namespace bWidgetsDemo {

class FontGlyph;

class Font {
public:
	~Font();

	static void initFontReading();
	static Font* loadFont(const std::string& name, const std::string& path);

	void render(const std::string& text, const int pos_x, const int pos_y);
	float renderGlyph(
	        const FontGlyph& glyph, const FontGlyph* previous_glyph,
	        const unsigned int attr_pos, const unsigned int attr_texcoord,
	        const int pos_x, const int pos_y) const;
	unsigned int calculateStringWidth(const std::string &text);

	void setSize(const float size);
	int getSize() const;

	const bWidgets::bwColor& getActiveColor() const;
	void setActiveColor(const bWidgets::bwColor &value);

	const bWidgets::bwRectanglePixel& getMask() const;
	void setMask(const bWidgets::bwRectanglePixel& value);

private:
	Font() = default;

	float getKerningDistance(const FontGlyph& left, const FontGlyph& right) const;

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


	class FontGlyphCache {
	// Everything public, this nested class is private to Font anyway.
	public:
		void ensureUpdated(Font&);
		const FontGlyph& getCachedGlyph(const Font&, const unsigned char) const;

		bool is_dirty{true};
		std::vector<std::unique_ptr<FontGlyph>> cached_glyphs;
	} cache;
};

class FontGlyph {
public:
	FontGlyph(
	        const unsigned int, const unsigned int, const unsigned int,
	        const int, const int, const int, const unsigned char*);
	FontGlyph();
	~FontGlyph();

	bool is_valid;

	unsigned int index; // Same as freetype index

	unsigned int width, height;  // width, rows
	int offset_left, offset_top; // bitmap_left, bitmap_top
	int advance_width; // advance.x

	unsigned char* bitmap;
};

} // namespace bWidgetsDemo
