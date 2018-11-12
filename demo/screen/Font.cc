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
#include FT_LCD_FILTER_H

#include "bwPoint.h"
#include "bwUtil.h"

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "FixedNum.h"
#include "ShaderProgram.h"

#include "Font.h"


using namespace bWidgetsDemo;

FT_Library Font::ft_library = nullptr;

namespace bWidgetsDemo {

class Pen
{
public:
	explicit Pen(FixedNum<F16p16> x = 0, FixedNum<F16p16> y = 0) : x(x), y(y) {}
	FixedNum<F16p16> x;
	FixedNum<F16p16> y;
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
	auto* font = new Font();
	FT_Face old_face = font->face;

	if (old_face) {
		FT_Done_Face(old_face);
	}
	if (FT_New_Face(ft_library, file_path.c_str(), 0, &font->face)) {
		std::cout << "Error: Failed to load font at " << file_path << "!" << std::endl;
	}

	return font;
}

static unsigned int getNumChannelsFromFreeTypePixelMode(FT_Pixel_Mode pixel_mode)
{
	switch (pixel_mode) {
		case FT_PIXEL_MODE_GRAY:
			return 1;
		case FT_PIXEL_MODE_LCD:
			return 3;
		default:
			assert(false);
			return 0;
	}
}

static unsigned int getGLFormatFromNumChannels(unsigned int num_channels)
{
	switch (num_channels) {
		case 1:
			return GL_RED;
		case 3:
			return GL_RGB;
		default:
			assert(false);
			return 0;
	}
}

void Font::render(const std::string& text, const int pos_x, const int pos_y)
{
	GLuint tex;
	ShaderProgram::ShaderProgramID program_id = render_mode == SUBPIXEL_LCD_RGB_COVERAGE ?
	                                                ShaderProgram::ID_SUBPIXEL_BITMAP_TEXTURE_UNIFORM_COLOR :
	                                                ShaderProgram::ID_BITMAP_TEXTURE_UNIFORM_COLOR;
	ShaderProgram& shader_program = ShaderProgram::getShaderProgram(program_id);
	VertexFormat* format = immVertexFormat();
	unsigned int pos = VertexFormat_add_attrib(format, "pos", COMP_F32, 2, KEEP_FLOAT);
	unsigned int texcoord = VertexFormat_add_attrib(format, "texCoord", COMP_F32, 2, KEEP_FLOAT);
	const FontGlyph* previous_glyph = nullptr;
	Pen pen(FixedNum<F16p16>::fromInt(pos_x), FixedNum<F16p16>::fromInt(pos_y));
	int old_scissor[4];

	cache.ensureUpdated(*this);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	immBindProgram(shader_program.ProgramID(), &shader_program.getInterface());
	immUniformColor4fv(active_color);

	glEnable(GL_BLEND);
	if (render_mode == Font::SUBPIXEL_LCD_RGB_COVERAGE) {
		glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR);
		glBlendColor(active_color[0], active_color[1], active_color[2], active_color[3]);
	}
	else {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (!mask.isEmpty()) {
		glGetIntegerv(GL_SCISSOR_BOX, old_scissor);
		glScissor(mask.xmin, mask.ymin, mask.width(), mask.height());
	}

	for (char character : text) {
		const FontGlyph& glyph = cache.getCachedGlyph(*this, character);

		if (!mask.isEmpty() && ((pen.x + glyph.advance_width) > FixedNum<F16p16>::fromInt(mask.xmax))) {
			break;
		}
		if (!glyph.is_valid) {
			std::cout << "Error: Trying to render invalid character" << std::endl;
		}

		renderGlyph(glyph, previous_glyph, pos, texcoord, pen);
		previous_glyph = &glyph;
	}

	if (!mask.isEmpty()) {
		glScissor(old_scissor[0], old_scissor[1], old_scissor[2], old_scissor[3]);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	glDeleteTextures(1, &tex);
	immUnbindProgram();
}

static void render_glyph_texture(
        const Pixmap& pixmap, const bWidgets::bwPoint& draw_pos,
        const unsigned int attr_pos, const unsigned int attr_texcoord)
{
	const unsigned int gl_format = getGLFormatFromNumChannels(pixmap.getNumChannels());
	const int w = pixmap.width();
	const int h = pixmap.height();

	// Could reduce this to one call per text render.
	if (pixmap.getNumChannels() == 1) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	else if (pixmap.getNumChannels() == 3) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, w, h, 0, gl_format, GL_UNSIGNED_BYTE, &pixmap.getBytes()[0]);

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
}

float Font::calcSubpixelOffset(const Pen& pen, const FontGlyph* previous_glyph) const
{
	if (use_tight_positioning) {
		return previous_glyph ? (float)previous_glyph->advance_width.getFractionAsReal() : 0.0f;
	}
	else {
		return pen.x.getFractionAsReal();
	}
}

void Font::applyPositionBias(FixedNum<F16p16>& value) const
{
	if (use_tight_positioning) {
		value.floor();
	}
	else if ((render_mode != SUBPIXEL_LCD_RGB_COVERAGE) || !use_subpixel_pos) {
		value.round();
	}
}

void Font::renderGlyph(
        const FontGlyph& glyph, const FontGlyph* previous_glyph,
        const unsigned int attr_pos, const unsigned int attr_texcoord,
        Pen& pen) const
{
	const Pixmap& pixmap = *glyph.pixmap;
	const bool has_texture = pixmap.getBytes().size() > 0;
	const bool use_kerning = previous_glyph != nullptr;

	if (use_kerning) {
		pen.x += getKerningDistance(*previous_glyph, glyph);
	}
	/* The actual position for drawing the bitmaps slightly differs from pen position. */
	bWidgets::bwPoint draw_pos((float)pen.x.toInt(), (float)pen.y.toInt());

	draw_pos.x += glyph.offset_left;
	draw_pos.y += glyph.offset_top;

	if (render_mode == SUBPIXEL_LCD_RGB_COVERAGE) {
		immUniform1f("subpixel_offset", use_subpixel_pos ? calcSubpixelOffset(pen, previous_glyph) : 0.0);
	}

	if (has_texture) {
		render_glyph_texture(pixmap, draw_pos, attr_pos, attr_texcoord);
	}

	pen.x += glyph.advance_width;
	applyPositionBias(pen.x);
}

void Font::setFontAntiAliasingMode(Font::AntiAliasingMode new_aa_mode)
{
	if (new_aa_mode != render_mode) {
		render_mode = new_aa_mode;
		cache.invalidate();
	}
}

void Font::setTightPositioning(bool value)
{
	if (value != use_tight_positioning) {
		use_tight_positioning = value;
		cache.invalidate();
	}
}

void Font::setHinting(bool value)
{
	if (value != use_hinting) {
		use_hinting = value;
		cache.invalidate();
	}
}

void Font::setSubPixelPositioning(bool value)
{
	if (value != use_subpixel_pos) {
		use_subpixel_pos = value;
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

FixedNum<F16p16> Font::getKerningDistance(const FontGlyph& left, const FontGlyph& right) const
{
	FT_Vector kerning_dist_xy;
	FT_Get_Kerning(face, left.index, right.index, FT_KERNING_DEFAULT, &kerning_dist_xy);
	FixedNum<F16p16> kerning_dist_fp(FixedNum<F26p6>(kerning_dist_xy.x));
	applyPositionBias(kerning_dist_fp);
	return kerning_dist_fp;
}

unsigned int Font::calculateStringWidth(const std::string& text)
{
	FixedNum<F16p16> width;

	cache.ensureUpdated(*this);

	const FontGlyph* prev_glyph = nullptr;
	for (char character : text) {
		const FontGlyph& glyph = cache.getCachedGlyph(*this, character);

		if (prev_glyph) {
			width += getKerningDistance(*prev_glyph, glyph);
		}

		width += glyph.advance_width;
		applyPositionBias(width);
		prev_glyph = &glyph;
	}

	return width.toInt();
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
FT_Int32 Font::getFreeTypeLoadFlags() const
{
	FT_Int32 load_flags = FT_LOAD_DEFAULT;

	// Hinting
	if (!use_hinting) {
		load_flags |= FT_LOAD_NO_HINTING;
	}

	// Subpixel rendering
	switch (render_mode) {
		case NORMAL_COVERAGE:
			load_flags |= FT_LOAD_TARGET_LIGHT;
			break;
		case SUBPIXEL_LCD_RGB_COVERAGE:
			load_flags |= FT_LOAD_TARGET_LCD;
			break;
	}

	return load_flags;
}

static bWidgets::bwPtr<Pixmap> createGlyphPixmap(FT_GlyphSlot freetype_glyph)
{
	const unsigned int num_channels  = getNumChannelsFromFreeTypePixelMode((FT_Pixel_Mode)freetype_glyph->bitmap.pixel_mode);
	Pixmap pixmap(freetype_glyph->bitmap.width / num_channels, freetype_glyph->bitmap.rows, num_channels,
	              8, abs(freetype_glyph->bitmap.pitch) - freetype_glyph->bitmap.width);

	pixmap.fill(freetype_glyph->bitmap.buffer);

	return bWidgets::bwPtr_new<Pixmap>(std::move(pixmap));
}

void Font::FontGlyphCache::loadGlyphsIntoCache(const Font& font)
{
	FT_UInt glyph_index;
	bWidgets::bwPtr<FontGlyph> glyph;

	for (FT_ULong charcode = FT_Get_First_Char(font.face, &glyph_index);
	     glyph_index != 0;
	     charcode = FT_Get_Next_Char(font.face, charcode, &glyph_index))
	{
		FT_Int32 load_flags = font.getFreeTypeLoadFlags();
		FT_Error error = FT_Load_Glyph(font.face, glyph_index, load_flags | FT_LOAD_RENDER);

		if (error != 0) {
			// This constructor marks glyph as invalid.
			glyph = bWidgets::bwPtr_new<FontGlyph>();
		}
		else {
			FT_GlyphSlot ft_glyph = font.face->glyph;
			FixedNum<F16p16> advance = ft_glyph->linearHoriAdvance;

			if ((font.render_mode == Font::SUBPIXEL_LCD_RGB_COVERAGE) && font.use_subpixel_pos) {
				advance += FixedNum<F26p6>(ft_glyph->rsb_delta - ft_glyph->lsb_delta);
			}

			glyph = bWidgets::bwPtr_new<FontGlyph>(
			            glyph_index,
			            createGlyphPixmap(ft_glyph),
			            ft_glyph->bitmap_left, ft_glyph->bitmap_top,
			            advance);
			glyph->pitch = ft_glyph->bitmap.pitch;
		}
		cached_glyphs[glyph_index] = std::move(glyph);
	}
}

void Font::FontGlyphCache::ensureUpdated(const Font& font)
{
	if (!is_dirty) {
		return;
	}

	cached_glyphs.clear();
	cached_glyphs.reserve(font.face->num_glyphs);
	/* make sure vector size matches num_glyphs and fill all entries with nullptr */
	for (int i = 0; i < font.face->num_glyphs; i++) {
		cached_glyphs.push_back(nullptr);
	}

#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
	if (font.render_mode == SUBPIXEL_LCD_RGB_COVERAGE) {
		/* FT_CONFIG_OPTION_SUBPIXEL_RENDERING enables patented ClearType
		 * subpixel rendering, which requires filtering to reduce color
		 * fringes. The used FreeType version may be a custom build with this
		 * option enabled (at the user's own risk), apply filtering for them. */
		FT_Error error = FT_Library_SetLcdFilter(ft_library, FT_LCD_FILTER_DEFAULT);
		assert(error == FT_Err_Ok);
	}
#endif

	loadGlyphsIntoCache(font);

	is_dirty = false;
}

const FontGlyph& Font::FontGlyphCache::getCachedGlyph(const Font& font, const char character) const
{
	return *cached_glyphs[FT_Get_Char_Index(font.face, static_cast<unsigned char>(character))];
}

FontGlyph::FontGlyph(
        const unsigned int index,
        bWidgets::bwPtr<Pixmap>&& pixmap,
        const int offset_left, const int offset_top,
        FixedNum<F16p16> advance_width) :
    is_valid(true),
    index(index),
    pixmap(std::move(pixmap)),
    offset_left(offset_left), offset_top(offset_top),
    advance_width(advance_width)
{
}
