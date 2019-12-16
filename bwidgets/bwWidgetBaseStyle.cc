#include <cassert>

#include "bwPainter.h"
#include "bwStyleProperties.h"

#include "bwWidgetBaseStyle.h"

using namespace bWidgets;

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

const bwColor& bwWidgetBaseStyle::backgroundColor() const
{
  return background_color;
}

const bwColor& bwWidgetBaseStyle::textColor() const
{
  return text_color;
}

const bwColor& bwWidgetBaseStyle::borderColor() const
{
  return border_color;
}

const bwColor& bwWidgetBaseStyle::decorationColor() const
{
  return decoration_color;
}

float bwWidgetBaseStyle::shadeTop() const
{
  return shade_top / 255.0f;
}

float bwWidgetBaseStyle::shadeBottom() const
{
  return shade_bottom / 255.0f;
}
