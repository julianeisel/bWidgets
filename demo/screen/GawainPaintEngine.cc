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
#include <cassert>

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
  const float width = (rect.width() + 1) * m_scale_x;
  const float height = (rect.height() + 1) * m_scale_y;

  glViewport(rect.xmin * m_scale_x, rect.ymin * m_scale_y, width, height);
  glScissor(rect.xmin * m_scale_x, rect.ymin * m_scale_y, width, height);

  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  gpuOrtho(0, width, 0, height);
  gpuIdentityMatrix();
}

void GawainPaintEngine::enableMask(const bwRectanglePixel& rect)
{
  glScissor(rect.xmin * m_scale_x,
            rect.ymin * m_scale_y,
            (rect.width() + 1) * m_scale_x,
            (rect.height() + 1) * m_scale_y);
}

// --------------------------------------------------------------------
// Polygon Drawing

#define WIDGET_AA_JITTER 8

static const float jit[WIDGET_AA_JITTER][2] = {{0.468813f, -0.481430f},
                                               {-0.155755f, -0.352820f},
                                               {0.219306f, -0.238501f},
                                               {-0.393286f, -0.110949f},
                                               {-0.024699f, 0.013908f},
                                               {0.343805f, 0.147431f},
                                               {-0.272855f, 0.269918f},
                                               {0.095909f, 0.388710f}};

static auto stage_polygon_drawtype_convert(const bwPainter::DrawType& drawtype,
                                           bool use_antialiasing) -> Gwn_PrimType
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
                                             const unsigned int attr_pos,
                                             float scale_x,
                                             float scale_y)
{
  const bwPointVec& vertices = poly.getVertices();

  immUniformColor4fv(color);

  immBegin(type, unsigned int(vertices.size()));
  for (const bwPoint& vertex : vertices) {
    immVertex2f(attr_pos, vertex.x * scale_x, vertex.y * scale_y);
  }
  immEnd();
}
static void stage_polygon_draw_shaded(const bwPainter& painter,
                                      const bwPolygon& poly,
                                      const Gwn_PrimType type,
                                      const unsigned int attr_pos,
                                      const unsigned int attr_color,
                                      float scale_x,
                                      float scale_y)
{
  const bwPointVec& vertices = poly.getVertices();

  immBegin(type, unsigned int(vertices.size()));
  for (int i = 0; i < vertices.size(); i++) {
    immAttrib4fv(attr_color, painter.getVertexColor(i));
    immVertex2f(attr_pos, vertices[i].x * scale_x, vertices[i].y * scale_y);
  }
  immEnd();
}
static void stage_polygon_draw(const bwPainter& painter,
                               const bwPolygon& poly,
                               const bwColor& color,
                               const Gwn_PrimType type,
                               const unsigned int attr_pos,
                               const unsigned int attr_color,
                               float scale_x,
                               float scale_y)
{
  if (painter.isGradientEnabled()) {
    stage_polygon_draw_shaded(painter, poly, type, attr_pos, attr_color, scale_x, scale_y);
  }
  else {
    stage_polygon_draw_uniform_color(poly, color, type, attr_pos, scale_x, scale_y);
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
      stage_polygon_draw(
          painter, poly, drawcolor, prim_type, attr_pos, attr_color, m_scale_x, m_scale_y);
      gpuTranslate2f(-i[0], -i[1]);
    }
  }
  else {
    stage_polygon_draw(
        painter, poly, color, prim_type, attr_pos, attr_color, m_scale_x, m_scale_y);
  }

  GPUShader::immUnbind();
  glDisable(GL_BLEND);
}

// --------------------------------------------------------------------
// Text Drawing

static auto stage_text_xpos_calc(Font& font,
                                 const std::string& text,
                                 const bwRectanglePixel& rectangle,
                                 const TextAlignment alignment,
                                 float scale_x) -> float
{
  int value = 0;

  switch (alignment) {
    case TextAlignment::LEFT:
      // XXX -9 is ugly. Goes out of widget rectangle even.
      value = (rectangle.xmin + 9) * scale_x;
      break;
    case TextAlignment::CENTER:
      value = rectangle.centerX() * scale_x - (font.calculateStringWidth(text) / 2.0f);
      break;
    case TextAlignment::RIGHT:
      // XXX -9 is ugly. Goes out of widget rectangle even.
      value = (rectangle.xmax - 9) * scale_x - font.calculateStringWidth(text);
      break;
  }

  return value;
}

void GawainPaintEngine::drawText(const bwPainter& painter,
                                 const std::string& text,
                                 const bwRectanglePixel& rectangle,
                                 const TextAlignment alignment)
{
  bwRectanglePixel scaled_mask = painter.getContentMask();
  const float font_height = font.getSize();
  const float draw_pos_x = stage_text_xpos_calc(font, text, rectangle, alignment, m_scale_x);
  const float draw_pos_y = (rectangle.centerY() + 1.0f) * m_scale_y - (font_height / 2.0f);

  scaled_mask.xmin *= m_scale_x;
  scaled_mask.xmax *= m_scale_x;
  scaled_mask.ymin *= m_scale_y;
  scaled_mask.ymax *= m_scale_y;
  font.setActiveColor(painter.getActiveColor());
  font.setMask(scaled_mask);
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
                                         const Pixmap& pixmap,
                                         float scale_x,
                                         float scale_y)
{
  const int xmin = std::max(bounds.centerX() - (pixmap.width() / 2) + 4, bounds.xmin);
  const int ymin = std::max(bounds.centerY() - (pixmap.height() / 2) + 1, bounds.ymin);

  icon_rect.set(xmin * scale_x,
                std::min(pixmap.width(), bounds.width()) * scale_x,
                ymin * scale_y,
                std::min(pixmap.height(), bounds.height()) * scale_y);
}

void GawainPaintEngine::drawIcon(const bwPainter& /*painter*/,
                                 const bwIconInterface& icon_interface,
                                 const bwRectanglePixel& rectangle)
{
  const auto& icon = static_cast<const Icon&>(icon_interface);
  const Pixmap& pixmap = icon.getPixmap();
  bwRectanglePixel icon_rect;
  GLuint texture_id = 0;

  engine_icon_rectangle_adjust(icon_rect, rectangle, pixmap, m_scale_x, m_scale_y);

  engine_icon_texture_drawing_prepare(pixmap, texture_id);
  engine_icon_texture_draw(icon_rect);
  engine_icon_texture_drawing_cleanup(texture_id);
}

}  // namespace bWidgetsDemo
