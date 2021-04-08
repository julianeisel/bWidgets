#include <cassert>

#include "bwPainter.h"
#include "bwStyleProperties.h"

#include "bwWidgetBaseStyle.h"

namespace bWidgets {

bwWidgetBaseStyle::bwWidgetBaseStyle()
    : text_alignment(TextAlignment::LEFT), roundbox_corners(NONE)
{
}

/**
 * Add bwStyleProperty instances to \a style_properties for all
 * bwWidgetBaseStyle options.
 */
void bwWidgetBaseStyle::registerProperties(bwStyleProperties& style_properties)
{
  style_properties.addColor("color", text_color);
  style_properties.addColor("background-color", background_color);
  style_properties.addColor("decoration-color", decoration_color);
  style_properties.addColor("border-color", border_color);

  style_properties.addInteger("shade-top", shade_top);
  style_properties.addInteger("shade-bottom", shade_bottom);

  style_properties.addFloat("border-radius", corner_radius);
}

auto bwWidgetBaseStyle::backgroundColor() const -> const bwColor&
{
  return background_color;
}

auto bwWidgetBaseStyle::textColor() const -> const bwColor&
{
  return text_color;
}

auto bwWidgetBaseStyle::borderColor() const -> const bwColor&
{
  return border_color;
}

auto bwWidgetBaseStyle::decorationColor() const -> const bwColor&
{
  return decoration_color;
}

auto bwWidgetBaseStyle::shadeTop() const -> float
{
  return shade_top / 255.0f;
}

auto bwWidgetBaseStyle::shadeBottom() const -> float
{
  return shade_bottom / 255.0f;
}

auto bwWidgetBaseStyle::isBorderVisible() const -> bool
{
  return (background_color == border_color) == false;
}

}  // namespace bWidgets
