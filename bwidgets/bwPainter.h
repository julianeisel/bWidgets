#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwIconInterface.h"
#include "bwGradient.h"
#include "bwUtil.h"

namespace bWidgets {

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
  } active_drawtype;

  static bwPtr<class bwPaintEngine> paint_engine;

  bwPainter();

  void drawPolygon(const class bwPolygon& poly);
  void drawText(const std::string& text,
                const bwRectanglePixel& rectangle,
                const TextAlignment align) const;
  void drawIcon(const bwIconInterface&, const bwRectanglePixel&) const;

  void setActiveColor(const bwColor& color);
  const bwColor& getActiveColor() const;
  const bwColor& getVertexColor(const size_t vertex_index) const;

  void setContentMask(const bwRectanglePixel& value);
  const bwRectanglePixel& getContentMask() const;

  void enableGradient(const bwGradient& gradient);
  bool isGradientEnabled() const;

  void drawTextAndIcon(const std::string& text,
                       const bwIconInterface* icon,
                       const bwRectanglePixel& rectangle,
                       const TextAlignment alignment,
                       float dpi_fac) const;

  // Primitives
  void drawRoundbox(const bwRectanglePixel& rect, unsigned int corners, const float radius);
  void drawRectangle(const bwRectanglePixel& rect);
  void drawRoundboxWidgetBase(const class bwWidgetBaseStyle& base_style,
                              const class bwStyle& style,
                              const bwRectanglePixel& rectangle,
                              const bwGradient& gradient,
                              const float radius);
  void drawCheckMark(const bwRectanglePixel& rect);
  void drawTriangle(const bwRectanglePixel& rect, Direction direction);
  void drawLine(const bwPoint& from, const bwPoint& to);

  bool use_antialiasing{false};

 private:
  bwColor active_color;
  std::vector<bwColor> vert_colors;
  bwPtr<bwGradient> active_gradient;
  bwRectanglePixel content_mask;

  void fillVertexColorsWithGradient(const bwPolygon& polygon,
                                    const bwRectanglePixel& bounding_box);
};

}  // namespace bWidgets
