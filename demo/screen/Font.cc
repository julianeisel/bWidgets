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

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "bwPoint.h"
#include "bwUtil.h"

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "ShaderProgram.h"

#include "Font.h"


using namespace bWidgetsDemo;

FT_Library Font::ft_library = nullptr;
bool Font::changed = false;

namespace bWidgetsDemo {

class Pen
{
public:
	explicit Pen(int x = 0, int y = 0) : position(x, y) {}
	bWidgets::bwPoint position;
};

} // namespace bWidgetsDemo


Font::~Font()
{
	FT_Done_Face(face);
	FT_Done_FreeType(ft_library);
}

void Font::initFontReading()
{
	if (FT_Init_FreeType(&ft_library)) {
		std::cout << "Error: Failed to initialize freetype library!" << std::endl;
		return;
	}
}

Font* Font::loadFont(const std::string& name, const std::string& path)
{
	std::string file_path(path + "/" + name);
	Font* font = new Font();
	FT_Face old_face = font->face;

	if (old_face) {
		FT_Done_Face(old_face);
	}
	if (FT_New_Face(ft_library, file_path.c_str(), 0, &font->face)) {
		std::cout << "Error: Failed to load font at " << file_path << "!" << std::endl;
	}
	if (font->face != old_face) {
		font->changed = true;
	}

	return font;
}

void Font::render(const std::string &text, const int pos_x, const int pos_y)
{
	GLuint tex;
	ShaderProgram& shader_program = ShaderProgram::getShaderProgram(ShaderProgram::ID_BITMAP_TEXTURE_UNIFORM_COLOR);
	VertexFormat* format = immVertexFormat();
	unsigned int pos = VertexFormat_add_attrib(format, "pos", COMP_F32, 2, KEEP_FLOAT);
	unsigned int texcoord = VertexFormat_add_attrib(format, "texCoord", COMP_F32, 2, KEEP_FLOAT);
	const FontGlyph* previous_glyph = nullptr;
	Pen pen(pos_x, pos_y);
	int old_scissor[4];

	cache.ensureUpdated(*this);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	immBindProgram(shader_program.ProgramID(), &shader_program.getInterface());
	immUniformColor4fv(active_color);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!mask.isEmpty()) {
		glGetIntegerv(GL_SCISSOR_BOX, old_scissor);
		glScissor(mask.xmin, mask.ymin, mask.width(), mask.height());
	}

	for (uint i = 0; i < text.size(); i++) {
		const FontGlyph& glyph = cache.getCachedGlyph(*this, text[i]);

		if (!mask.isEmpty() && ((pen.position.x + glyph.advance_width) > mask.xmax)) {
			break;
		}
		if (glyph.is_valid == false) {
			std::cout << "Error: Trying to render invalid character" << std::endl;
		}
		renderGlyph(glyph, previous_glyph, pos, texcoord, pen);
		previous_glyph = &glyph;
	}

	if (!mask.isEmpty()) {
		glScissor(old_scissor[0], old_scissor[1], old_scissor[2], old_scissor[3]);
	}

	glDisable(GL_BLEND);
	glDeleteTextures(1, &tex);
	immUnbindProgram();

	changed = false;
}

void Font::renderGlyph(
        const FontGlyph& glyph, const FontGlyph* previous_glyph,
        const unsigned int attr_pos, const unsigned int attr_texcoord,
        Pen& pen) const
{
	const float w = glyph.width;
	const float h = glyph.height;
	const bool use_kerning = previous_glyph != nullptr;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, glyph.bitmap);

	if (use_kerning) {
		pen.position.x += getKerningDistance(*previous_glyph, glyph);
	}

	/* The actual position for drawing the bitmaps slightly differs from pen position. */
	bWidgets::bwPoint draw_pos(pen.position);

	draw_pos.x += glyph.offset_left;
	draw_pos.y += glyph.offset_top;

	immBegin(PRIM_TRIANGLE_STRIP, 4);
	immAttrib2f(attr_texcoord, 0.0f, 0.0f);
	immVertex2f(attr_pos, draw_pos.x, draw_pos.y);
	immAttrib2f(attr_texcoord, 1.0f, 0.0f);
	immVertex2f(attr_pos, draw_pos.x + w, draw_pos.y);
	immAttrib2f(attr_texcoord, 0.0f, 1.0f);
	immVertex2f(attr_pos, draw_pos.x, draw_pos.y - h);
	immAttrib2f(attr_texcoord, 1.0f, 1.0f);
	immVertex2f(attr_pos, draw_pos.x + w, draw_pos.y - h);
	immEnd();

	pen.position.x += glyph.advance_width;
}

void Font::setHinting(bool value)
{
	if (value != use_hinting) {
		use_hinting = value;
		cache.invalidate();
	}
}

void Font::setSize(const float _size)
{
	size = _size;
	FT_Set_Pixel_Sizes(face, 0, size);
	cache.invalidate();
}

int Font::getSize() const
{
	return size;
}

const bWidgets::bwColor& Font::getActiveColor() const
{
	return active_color;
}

void Font::setActiveColor(const bWidgets::bwColor &value)
{
	active_color = value;
}

const bWidgets::bwRectanglePixel& Font::getMask() const
{
	return mask;
}

void Font::setMask(const bWidgets::bwRectanglePixel& value)
{
	mask = value;
}

float Font::getKerningDistance(const FontGlyph& left, const FontGlyph& right) const
{
	FT_Vector kerning_dist_xy;
	FT_Get_Kerning(face, left.index, right.index, FT_KERNING_DEFAULT, &kerning_dist_xy);
	return kerning_dist_xy.x / 64.0f;
}

unsigned int Font::calculateStringWidth(const std::string& text)
{
	unsigned int width = 0;

	cache.ensureUpdated(*this);

	const FontGlyph* prev_glyph = nullptr;
	for (uint i = 0; i < text.size(); i++) {
		const FontGlyph& glyph = cache.getCachedGlyph(*this, text[i]);

		if (prev_glyph) {
			width += getKerningDistance(*prev_glyph, glyph);
		}

		width += glyph.advance_width;
		prev_glyph = &glyph;
	}

	return width;
}

void Font::FontGlyphCache::invalidate()
{
	is_dirty = true;
	cached_glyphs.clear();
	cached_glyphs.resize(0);
}

/**
 * \return The flags that should be used for the FT_Load_Glyph call.
 */
FT_Int32 Font::getFreetypeLoadFlags()
{
	return use_hinting ? FT_LOAD_TARGET_LIGHT : FT_LOAD_NO_HINTING;
}

void Font::FontGlyphCache::ensureUpdated(Font& font)
{
	FT_UInt glyph_index;
	bWidgets::bwPointer<FontGlyph> glyph;

	if (is_dirty == false) {
		return;
	}

	cached_glyphs.clear();
	cached_glyphs.reserve(font.face->num_glyphs);
	/* make sure vector size matches num_glyphs and fill all entries with nullptr */
	for (int i = 0; i < font.face->num_glyphs; i++) {
		cached_glyphs.push_back(nullptr);
	}

	for (FT_ULong charcode = FT_Get_First_Char(font.face, &glyph_index);
	     glyph_index != 0;
	     charcode = FT_Get_Next_Char(font.face, charcode, &glyph_index))
	{
		FT_Int32 load_flags = font.getFreetypeLoadFlags();
		FT_Error error = FT_Load_Glyph(font.face, glyph_index, load_flags | FT_LOAD_RENDER);

		if (error != 0) {
			// This constructor marks glyph as invalid.
			glyph = bWidgets::bwPointer_new<FontGlyph>();
		}
		else {
			const FT_GlyphSlot freetype_glyph = font.face->glyph;

			glyph = bWidgets::bwPointer_new<FontGlyph>(
			            glyph_index,
			            freetype_glyph->bitmap.width, freetype_glyph->bitmap.rows,
			            freetype_glyph->bitmap_left, freetype_glyph->bitmap_top,
			            freetype_glyph->advance.x >> 6,
			            freetype_glyph->bitmap.buffer);
		}
		cached_glyphs[glyph_index] = std::move(glyph);
	}

	is_dirty = false;
}

const FontGlyph& Font::FontGlyphCache::getCachedGlyph(const Font& font, const unsigned char character) const
{
	return *cached_glyphs[FT_Get_Char_Index(font.face, character)];
}

FontGlyph::FontGlyph(
        const unsigned int index,
        const unsigned int width, const unsigned int height,
        const int offset_left, const int offset_top,
        const int advance_width,
        const unsigned char* bitmap_buffer) :
    is_valid(true),
    index(index),
    width(width), height(height),
    offset_left(offset_left), offset_top(offset_top),
    advance_width(advance_width)
{
	bitmap = new unsigned char[width * height];
	memcpy(bitmap, bitmap_buffer, width * height);
}

FontGlyph::FontGlyph() :
    is_valid(false)
{
	
}

FontGlyph::~FontGlyph()
{
	if (is_valid) {
		delete[] bitmap;
	}
}
