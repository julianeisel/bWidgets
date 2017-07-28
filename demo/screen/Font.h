#pragma once

#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Color.h"


namespace bWidgetDemo {

class Font {
public:
	static void initFontReading();
	static Font* loadFont(const std::string& name, const std::string& path);

	void render(const std::string& text, const int pos_x, const int pos_y);
	float renderCharacter(
	        unsigned char character,
	        const unsigned int attr_pos, const unsigned int attr_texcoord,
	        const int pos_x, const int pos_y) const;
	int calculateStringWidth(const std::string &text);

	void setSize(const float size);
	int getSize() const;

	const bWidgets::Color& getActiveColor() const;
	void setActiveColor(const bWidgets::Color &value);

private:
	Font() {}

	// The freetype library handle.
	static FT_Library ft_library;
	// The freetype font handle.
	FT_Face face;

	// Height in pixels.
	int size;

	bWidgets::Color active_color;


	class FontGlyphCache {
	// Everything public, this nested class is private to Font anyway.
	public:
		FontGlyphCache();

		void ensureUpdated(Font&);

		class CachedGlyph {
		public:
			CachedGlyph(const unsigned int, const unsigned int, const int, const int, const int, const unsigned char*);
			~CachedGlyph();

			unsigned int width, height;
			int offset_left, offset_top;
			int advance_width;

			unsigned char* bitmap;
		};
		const CachedGlyph& getCachedGlyph(const Font&, const unsigned char) const;

		bool is_dirty;
		std::vector<std::unique_ptr<CachedGlyph>> cached_glyphs;
	} cache;
};

} // namespace bWidgetDemo
