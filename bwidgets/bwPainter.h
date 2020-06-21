#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwGradient.h"
#include "bwIconInterface.h"

namespace bWidgets {

class bwPaintEngine;
class bwPolygon;
class bwStyle;
class bwWidgetBaseStyle;

enum RoundboxCorner {
  NONE = 0,
  BOTTOM_LEFT = (1 << 0),
  BOTTOM_RIGHT = (1 << 1),
  TOP_LEFT = (1 << 2),
  TOP_RIGHT = (1 << 3),
  /* Convenience */
  ALL = (BOTTOM_LEFT | BOTTOM_RIGHT | TOP_LEFT | TOP_RIGHT),
};

enum class TextAlignment {
  LEFT,
  CENTER,
  RIGHT,
};

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

class bwPainter {
 public:
  enum class DrawType {
    FILLED,
    OUTLINE,
    LINE,
  };

  bwPainter();

  void drawPolygon(const bwPolygon& poly);
  void drawText(const std::string& text,
                const bwRectanglePixel& rectangle,
                const TextAlignment align) const;
  void drawIcon(const bwIconInterface&, const bwRectanglePixel&) const;

  void setActiveColor(const bwColor& color);
  auto getActiveColor() const -> const bwColor&;
  auto getVertexColor(const size_t vertex_index) const -> const bwColor&;

  void setContentMask(const bwRectanglePixel& value);
  auto getContentMask() const -> const bwRectanglePixel&;

  void enableGradient(const bwGradient& gradient);
  auto isGradientEnabled() const -> bool;

  void drawTextAndIcon(const std::string& text,
                       const bwIconInterface* icon,
                       const bwRectanglePixel& rectangle,
                       const TextAlignment alignment,
                       float dpi_fac) const;

  // Primitives
  void drawRoundbox(const bwRectanglePixel& rect, unsigned int corners, const float radius);
  void drawRectangle(const bwRectanglePixel& rect);
  void drawRoundboxWidgetBase(const bwWidgetBaseStyle& base_style,
                              const bwStyle& style,
                              const bwRectanglePixel& rectangle,
                              const bwGradient& gradient,
                              const float radius);
  void drawCheckMark(const bwRectanglePixel& rect);
  void drawTriangle(const bwRectanglePixel& rect, Direction direction);
  void drawLine(const bwPoint& from, const bwPoint& to);

  static std::unique_ptr<bwPaintEngine> s_paint_engine;

  bool use_antialiasing{false};
  DrawType active_drawtype;

 private:
  void fillVertexColorsWithGradient(const bwPolygon& polygon,
                                    const bwRectanglePixel& bounding_box);

  bwColor active_color;
  std::vector<bwColor> vert_colors;
  std::unique_ptr<bwGradient> active_gradient;
  bwRectanglePixel content_mask;
};

}  // namespace bWidgets
