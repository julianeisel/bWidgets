#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "../gpu/ShaderProgram.h"

#include "Font.h"


using namespace bWidgetDemo;

FT_Library Font::ft_library = nullptr;


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

	if (FT_New_Face(ft_library, file_path.c_str(), 0, &font->face)) {
		std::cout << "Error: Failed to load font at " << file_path << "!" << std::endl;
	}

	return font;
}

void Font::render(const std::string &text, const int pos_x, const int pos_y)
{
	GLuint tex;
	ShaderProgram shader_program(ShaderProgram::ID_BITMAP_TEXTURE_UNIFORM_COLOR);
	VertexFormat* format = immVertexFormat();
	unsigned int pos = VertexFormat_add_attrib(format, "pos", COMP_F32, 2, KEEP_FLOAT);
	unsigned int texcoord = VertexFormat_add_attrib(format, "texCoord", COMP_F32, 2, KEEP_FLOAT);
	float render_pos_x = pos_x;

	cache.ensureUpdated(*this);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());
	immUniformColor4fv(active_color);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (unsigned char character : text) {
		render_pos_x += renderCharacter(character, pos, texcoord, render_pos_x, pos_y);
	}

	glDisable(GL_BLEND);
	immUnbindProgram();
}

/**
 * \return The offset at which the next character should be drawn.
 */
float Font::renderCharacter(
        unsigned char character,
        const unsigned int attr_pos, const unsigned int attr_texcoord,
        const int pos_x, const int pos_y) const
{
	const FontGlyphCache::CachedGlyph& glyph = cache.getCachedGlyph(*this, character);
	const float w = glyph.width;
	const float h = glyph.height;
	const float x = pos_x + glyph.offset_left;
	const float y = -pos_y - glyph.offset_top;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, glyph.bitmap);

	immBegin(PRIM_TRIANGLE_STRIP, 4);
	immAttrib2f(attr_texcoord, 0.0f, .0f);
	immVertex2f(attr_pos, x, -y);
	immAttrib2f(attr_texcoord, 1.0f, 0.0f);
	immVertex2f(attr_pos, x + w, -y);
	immAttrib2f(attr_texcoord, 0.0f, 1.0f);
	immVertex2f(attr_pos, x, -y - h);
	immAttrib2f(attr_texcoord, 1.0f, 1.0f);
	immVertex2f(attr_pos, x + w, -y - h);
	immEnd();

	return glyph.advance_width;
}

void Font::setSize(const float _size)
{
	size = _size;
	cache.is_dirty = true;
	cache.cached_glyphs.clear();
	cache.cached_glyphs.resize(0);
	FT_Set_Pixel_Sizes(face, 0, size);
}

int Font::getSize() const
{
	return size;
}

const bWidgets::Color& Font::getActiveColor() const
{
	return active_color;
}

void Font::setActiveColor(const bWidgets::Color &value)
{
	active_color = value;
}

unsigned int Font::calculateStringWidth(const std::string& text)
{
	unsigned int width = 0;

	cache.ensureUpdated(*this);

	for (unsigned char character : text) {
		const FontGlyphCache::CachedGlyph& glyph = cache.getCachedGlyph(*this, character);
		width += glyph.advance_width + glyph.offset_left;
	}

	return width;
}

Font::FontGlyphCache::FontGlyphCache() :
    is_dirty(true)
{
	
}

void Font::FontGlyphCache::ensureUpdated(Font& font)
{
	FT_UInt glyph_index;

	if (is_dirty == false) {
		return;
	}

	cached_glyphs.reserve(font.face->num_glyphs);
	/* make sure vector size matches num_glyphs and fill all entries with nullptr */
	for (int i = 0; i < font.face->num_glyphs; i++) {
		cached_glyphs.push_back(nullptr);
	}

	for (FT_ULong charcode = FT_Get_First_Char(font.face, &glyph_index);
	     glyph_index != 0;
	     charcode = FT_Get_Next_Char(font.face, charcode, &glyph_index))
	{
		if (FT_Load_Glyph(font.face, glyph_index, FT_LOAD_RENDER)) {
			std::cout << "Error: Failed to render character at index '" << glyph_index << "' into cache" << std::endl;
		}
		else {
			const FT_GlyphSlot freetype_glyph = font.face->glyph;
			std::unique_ptr<CachedGlyph> glyph(new CachedGlyph(
			                                         freetype_glyph->bitmap.width, freetype_glyph->bitmap.rows,
			                                         freetype_glyph->bitmap_left, freetype_glyph->bitmap_top,
			                                         (int)freetype_glyph->advance.x,
			                                         freetype_glyph->bitmap.buffer));

			cached_glyphs[glyph_index] = std::move(glyph);
		}
	}

	is_dirty = false;
}

const Font::FontGlyphCache::CachedGlyph& Font::FontGlyphCache::getCachedGlyph(
        const Font& font, const unsigned char character) const
{
	return *cached_glyphs[FT_Get_Char_Index(font.face, character)];
}

Font::FontGlyphCache::CachedGlyph::CachedGlyph(
        const unsigned int width, const unsigned int height,
        const int offset_left, const int offset_top,
        const int advance_width,
        const unsigned char* bitmap_buffer) :
    width(width), height(height),
    offset_left(offset_left), offset_top(offset_top),
    advance_width(advance_width / 64.0f)
{
	bitmap = new unsigned char[width * height];
	memcpy(bitmap, bitmap_buffer, width * height);
}

Font::FontGlyphCache::CachedGlyph::~CachedGlyph()
{
	delete[] bitmap;
}
