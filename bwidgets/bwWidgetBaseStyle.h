#pragma once

#include "bwColor.h"

namespace bWidgets {

enum class TextAlignment;
class bwStyleProperties;

/**
 * \class bwWidgetBaseStyle
 * \brief Properties for typical widgets with roundbox corners.
 */
class bwWidgetBaseStyle {
 public:
  bwWidgetBaseStyle();

  void registerProperties(bwStyleProperties& style_properties);

  auto backgroundColor() const -> const bwColor&;
  auto textColor() const -> const bwColor&;
  auto borderColor() const -> const bwColor&;
  auto decorationColor() const -> const bwColor&;
  auto shadeTop() const -> float;
  auto shadeBottom() const -> float;

  auto isBorderVisible() const -> bool;

  bwColor background_color;
  bwColor text_color;
  bwColor border_color;
  bwColor decoration_color;

  int shade_top{0};
  int shade_bottom{
      0};  // TODO could make this a char - or support gradient for background_color even.
  enum TextAlignment text_alignment;  // Default value set in constructor.
  unsigned int roundbox_corners;      // Default value set in constructor.
  float corner_radius{
      0.0f};  // TODO Utility function for calculating final radius (roundbox_radius * dpi_fac).
};

}  // namespace bWidgets
