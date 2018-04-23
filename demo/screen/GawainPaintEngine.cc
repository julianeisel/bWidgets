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
 * Original work Copyright (c) 2009, 2018 Blender Foundation, Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "Font.h"
#include "GPU.h"
#include "ShaderProgram.h"

#include "bwPainter.h"
#include "bwPolygon.h"

#include "GawainPaintEngine.h"

using namespace bWidgetsDemo;
using namespace bWidgets; // less verbose


GawainPaintEngine::GawainPaintEngine(Font& font) :
    font(font)
{
	
}

void GawainPaintEngine::setupViewport(
        const bwRectanglePixel& rect,
        const bWidgets::bwColor& clear_color)
{
	glViewport(rect.xmin, rect.ymin, rect.width(), rect.height());
	glScissor(rect.xmin, rect.ymin, rect.width(), rect.height());

	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	gpuOrtho(rect.xmin, rect.xmax, rect.ymin, rect.ymax);
}


// --------------------------------------------------------------------
// Polygon Drawing

#define WIDGET_AA_JITTER 8

static const float jit[WIDGET_AA_JITTER][2] = {
	{ 0.468813, -0.481430}, {-0.155755, -0.352820},
	{ 0.219306, -0.238501}, {-0.393286, -0.110949},
	{-0.024699,  0.013908}, { 0.343805,  0.147431},
	{-0.272855,  0.269918}, { 0.095909,  0.388710}
};

static PrimitiveType stage_polygon_drawtype_convert(
        const bwPainter::DrawType& drawtype)
{
	switch (drawtype) {
		case bwPainter::DRAW_TYPE_FILLED:
			return PRIM_TRIANGLE_FAN;
		case bwPainter::DRAW_TYPE_OUTLINE:
			return PRIM_TRIANGLE_STRIP;
		case bwPainter::DRAW_TYPE_LINE:
			return PRIM_LINE_STRIP;
	}

	return PRIM_NONE;
}

static void stage_polygon_draw_uniform_color(
        const bwPolygon& poly,
        const bwColor& color,
        const PrimitiveType type,
        const unsigned int attr_pos)
{
	const bwPointVec& vertices = poly.getVertices();

	immUniformColor4fv(color);

	immBegin(type, vertices.size());
	for (const bwPoint& vertex : vertices) {
		immVertex2f(attr_pos, vertex.x, vertex.y);
	}
	immEnd();
}
static void stage_polygon_draw_shaded(
        const bwPainter& painter,
        const bwPolygon& poly,
        const PrimitiveType type,
        const unsigned int attr_pos, const unsigned int attr_color)
{
	const bwPointVec& vertices = poly.getVertices();

	immBegin(type, vertices.size());
	for (int i = 0; i < vertices.size(); i++) {
		immAttrib4fv(attr_color, painter.getVertexColor(i));
		immVertex2f(attr_pos, vertices[i].x, vertices[i].y);
	}
	immEnd();
}
static void stage_polygon_draw(
        const bwPainter& painter,
        const bwPolygon& poly,
        const bwColor& color,
        const PrimitiveType type,
        const unsigned int attr_pos, const unsigned int attr_color)
{
	if (painter.isGradientEnabled()) {
		stage_polygon_draw_shaded(painter, poly, type, attr_pos, attr_color);
	}
	else {
		stage_polygon_draw_uniform_color(poly, color, type, attr_pos);
	}
}

void GawainPaintEngine::drawPolygon(
        const bwPainter& painter,
        const bwPolygon& poly)
{
	const bool is_shaded = painter.isGradientEnabled();
	ShaderProgram& shader_program = ShaderProgram::getShaderProgram(is_shaded ?
	                                                                    ShaderProgram::ID_SMOOTH_COLOR :
	                                                                    ShaderProgram::ID_UNIFORM_COLOR);
	const bwColor& color = painter.getActiveColor();
	PrimitiveType prim_type = stage_polygon_drawtype_convert(painter.active_drawtype);
	VertexFormat* format = immVertexFormat();
	unsigned int attr_pos = VertexFormat_add_attrib(format, "pos", COMP_F32, 2, KEEP_FLOAT);
	unsigned int attr_color = is_shaded ? VertexFormat_add_attrib(format, "color", COMP_F32, 4, KEEP_FLOAT) : 0;


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());

	if (painter.active_drawtype == bwPainter::DRAW_TYPE_OUTLINE || painter.use_antialiasing) {
		bwColor drawcolor = color;

		drawcolor[3] /= WIDGET_AA_JITTER;

		for (int i = 0; i < WIDGET_AA_JITTER; i++) {
			gpuTranslate2f(jit[i]);
			stage_polygon_draw(painter, poly, drawcolor, prim_type, attr_pos, attr_color);
			gpuTranslate2f(-jit[i][0], -jit[i][1]);
		}
	}
	else {
		stage_polygon_draw(painter, poly, color, prim_type, attr_pos, attr_color);
	}

	immUnbindProgram();
	glDisable(GL_BLEND);
}


// --------------------------------------------------------------------
// Text Drawing

static float stage_text_xpos_calc(
        Font& font,
        const std::string& text,
        const bwRectanglePixel& rectangle,
        const TextAlignment alignment)
{
	int value = 0;

	switch (alignment) {
		case TEXT_ALIGN_LEFT:
			value = rectangle.xmin + 10; // XXX +10 is ugly. Goes out of widget rectangle even.
			break;
		case TEXT_ALIGN_CENTER:
			value = rectangle.centerX() - (font.calculateStringWidth(text) / 2.0f);
			break;
		case TEXT_ALIGN_RIGHT:
			value = rectangle.xmax - font.calculateStringWidth(text) - 10; // XXX -10 is ugly. Goes out of widget rectangle even.
			break;
	}

	return value;
}

void GawainPaintEngine::drawText(
        const bwPainter& painter,
        const std::string& text,
        const bwRectanglePixel& rectangle,
        const TextAlignment alignment)
{
	const float font_height = font.getSize();
	const float draw_pos_x = stage_text_xpos_calc(font, text, rectangle, alignment);
	const float draw_pos_y = rectangle.centerY() - (font_height / 2.0f) + 1.0f;

	font.setActiveColor(painter.getActiveColor());
	font.setMask(painter.getContentMask());
	font.render(text, draw_pos_x, draw_pos_y);
}
