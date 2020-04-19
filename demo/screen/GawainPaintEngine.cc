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

#include <cmath>

extern "C" {
#include "gawain/gwn_immediate.h"
}
#include "Font.h"
#include "GPU.h"
#include "GPUShader.h"
#include "IconMap.h"

#include "bwPainter.h"
#include "bwPoint.h"
#include "bwPolygon.h"

#include "GawainPaintEngine.h"

using namespace bWidgets;  // less verbose

namespace bWidgetsDemo {

GawainPaintEngine::GawainPaintEngine(Font& font, IconMap& icon_map)
    : font(font), icon_map(icon_map)
{
}

void GawainPaintEngine::setupViewport(const bwRectanglePixel& rect, const bwColor& clear_color)
{
  const float width = rect.width() + 1;
  const float height = rect.height() + 1;

  glViewport(rect.xmin, rect.ymin, width, height);
  glScissor(rect.xmin, rect.ymin, width, height);

  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  gpuOrtho(0, width, 0, height);
  gpuIdentityMatrix();
}

void GawainPaintEngine::enableMask(const bwRectanglePixel& rect)
{
  glScissor(rect.xmin, rect.ymin, rect.width() + 1, rect.height() + 1);
}

// --------------------------------------------------------------------
// Polygon Drawing

#define WIDGET_AA_JITTER 8

static const float jit[WIDGET_AA_JITTER][2] = {{0.468813, -0.481430},
                                               {-0.155755, -0.352820},
                                               {0.219306, -0.238501},
                                               {-0.393286, -0.110949},
                                               {-0.024699, 0.013908},
                                               {0.343805, 0.147431},
                                               {-0.272855, 0.269918},
                                               {0.095909, 0.388710}};

static Gwn_PrimType stage_polygon_drawtype_convert(const bwPainter::DrawType& drawtype,
                                                   bool use_antialiasing)
{
  switch (drawtype) {
    case bwPainter::DrawType::FILLED:
      return GWN_PRIM_TRI_FAN;
    case bwPainter::DrawType::OUTLINE:
      return use_antialiasing ? GWN_PRIM_TRI_STRIP : GWN_PRIM_LINE_LOOP;
    case bwPainter::DrawType::LINE:
      return GWN_PRIM_LINE_STRIP;
  }

  return GWN_PRIM_NONE;
}

static void stage_polygon_draw_uniform_color(const bwPolygon& poly,
                                             const bwColor& color,
                                             const Gwn_PrimType type,
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
static void stage_polygon_draw_shaded(const bwPainter& painter,
                                      const bwPolygon& poly,
                                      const Gwn_PrimType type,
                                      const unsigned int attr_pos,
                                      const unsigned int attr_color)
{
  const bwPointVec& vertices = poly.getVertices();

  immBegin(type, vertices.size());
  for (int i = 0; i < vertices.size(); i++) {
    immAttrib4fv(attr_color, painter.getVertexColor(i));
    immVertex2f(attr_pos, vertices[i].x, vertices[i].y);
  }
  immEnd();
}
static void stage_polygon_draw(const bwPainter& painter,
                               const bwPolygon& poly,
                               const bwColor& color,
                               const Gwn_PrimType type,
                               const unsigned int attr_pos,
                               const unsigned int attr_color)
{
  if (painter.isGradientEnabled()) {
    stage_polygon_draw_shaded(painter, poly, type, attr_pos, attr_color);
  }
  else {
    stage_polygon_draw_uniform_color(poly, color, type, attr_pos);
  }
}

void GawainPaintEngine::drawPolygon(const bwPainter& painter, const bwPolygon& poly)
{
  const bool is_shaded = painter.isGradientEnabled();
  const bwColor& color = painter.getActiveColor();
  Gwn_PrimType prim_type = stage_polygon_drawtype_convert(painter.active_drawtype,
                                                          painter.use_antialiasing);
  Gwn_VertFormat* format = immVertexFormat();
  unsigned int attr_pos = GWN_vertformat_attr_add(format, "pos", GWN_COMP_F32, 2, GWN_FETCH_FLOAT);
  unsigned int attr_color = is_shaded ? GWN_vertformat_attr_add(
                                            format, "color", GWN_COMP_F32, 4, GWN_FETCH_FLOAT) :
                                        0;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  GPUShader::immBind(is_shaded ? GPUShader::ID_SMOOTH_COLOR : GPUShader::ID_UNIFORM_COLOR);

  if (painter.use_antialiasing) {
    bwColor drawcolor = color;

    drawcolor[3] /= WIDGET_AA_JITTER;

    for (const float* i : jit) {
      gpuTranslate2f(i);
      stage_polygon_draw(painter, poly, drawcolor, prim_type, attr_pos, attr_color);
      gpuTranslate2f(-i[0], -i[1]);
    }
  }
  else {
    stage_polygon_draw(painter, poly, color, prim_type, attr_pos, attr_color);
  }

  GPUShader::immUnbind();
  glDisable(GL_BLEND);
}

// --------------------------------------------------------------------
// Text Drawing

static float stage_text_xpos_calc(Font& font,
                                  const std::string& text,
                                  const bwRectanglePixel& rectangle,
                                  const TextAlignment alignment)
{
  int value = 0;

  switch (alignment) {
    case TextAlignment::LEFT:
      value = rectangle.xmin + 9;  // XXX +9 is ugly. Goes out of widget rectangle even.
      break;
    case TextAlignment::CENTER:
      value = rectangle.centerX() - (font.calculateStringWidth(text) / 2.0f);
      break;
    case TextAlignment::RIGHT:
      value = rectangle.xmax - font.calculateStringWidth(text) -
              9;  // XXX -9 is ugly. Goes out of widget rectangle even.
      break;
  }

  return value;
}

void GawainPaintEngine::drawText(const bwPainter& painter,
                                 const std::string& text,
                                 const bwRectanglePixel& rectangle,
                                 const TextAlignment alignment)
{
  const float font_height = font.getSize();
  const float draw_pos_x = stage_text_xpos_calc(font, text, rectangle, alignment);
  const float draw_pos_y = rectangle.centerY() - (font_height / 2.0f) + 1.0f;

  font.setActiveColor(painter.getActiveColor());
  font.setMask(painter.getContentMask());
  font.render(text, std::floor(draw_pos_x), std::floor(draw_pos_y));
}

// --------------------------------------------------------------------
// Icon Drawing

static void engine_icon_texture_draw(const bwRectanglePixel& icon_rect)
{
  Gwn_VertFormat* format = immVertexFormat();
  unsigned int pos = GWN_vertformat_attr_add(format, "pos", GWN_COMP_F32, 2, GWN_FETCH_FLOAT);
  unsigned int texcoord = GWN_vertformat_attr_add(
      format, "texCoord", GWN_COMP_F32, 2, GWN_FETCH_FLOAT);

  GPUShader::immBind(GPUShader::ID_TEXTURE_RECT);
  immUniformColor4fv(bwColor(1.0f, 1.0f));
  immUniform1i("image", 0);

  immBegin(GWN_PRIM_TRI_STRIP, 4);

  immAttrib2f(texcoord, 0.0f, 0.0f);
  immVertex2f(pos, icon_rect.xmin, icon_rect.ymin);

  immAttrib2f(texcoord, 1.0f, 0.0f);
  immVertex2f(pos, icon_rect.xmax, icon_rect.ymin);

  immAttrib2f(texcoord, 0.0f, 1.0f);
  immVertex2f(pos, icon_rect.xmin, icon_rect.ymax);

  immAttrib2f(texcoord, 1.0f, 1.0f);
  immVertex2f(pos, icon_rect.xmax, icon_rect.ymax);

  immEnd();
  GPUShader::immUnbind();
}

/**
 * Enables necessary GL states, generates and binds the texture.
 */
static void engine_icon_texture_drawing_prepare(const Pixmap& pixmap, GLuint& texture_id)
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glGenTextures(1, &texture_id);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // TODO should consider channel bit depth.
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               pixmap.width(),
               pixmap.height(),
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               pixmap.getBytes().data());
}
static void engine_icon_texture_drawing_cleanup(GLuint texture_id)
{
  assert(texture_id != 0);

  glDisable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture_id);
}

/**
 * Makes \a icon_rect use dimensions of \a icon, but centers it and clips it
 * within \a bounds.
 */
static void engine_icon_rectangle_adjust(bwRectanglePixel& icon_rect,
                                         const bwRectanglePixel& bounds,
                                         const Pixmap& pixmap)
{
  const int xmin = std::max(bounds.centerX() - (pixmap.width() / 2) + 4, bounds.xmin);
  const int ymin = std::max(bounds.centerY() - (pixmap.height() / 2) + 1, bounds.ymin);

  icon_rect.set(xmin,
                std::min(pixmap.width(), bounds.width()),
                ymin,
                std::min(pixmap.height(), bounds.height()));
}

void GawainPaintEngine::drawIcon(const bwPainter& /*painter*/,
                                 const bwIconInterface& icon_interface,
                                 const bwRectanglePixel& rectangle)
{
  const auto& icon = static_cast<const Icon&>(icon_interface);
  const Pixmap& pixmap = icon.getPixmap();
  bwRectanglePixel icon_rect;
  GLuint texture_id = 0;

  engine_icon_rectangle_adjust(icon_rect, rectangle, pixmap);

  engine_icon_texture_drawing_prepare(pixmap, texture_id);
  engine_icon_texture_draw(icon_rect);
  engine_icon_texture_drawing_cleanup(texture_id);
}

}  // namespace bWidgetsDemo
