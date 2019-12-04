#include <cmath>

#include "bwPainter.h"
#include "bwStyle.h"

#include "bwLabel.h"

using namespace bWidgets;

bwLabel::bwLabel(std::string text, unsigned int width_hint, unsigned int height_hint)
    : bwWidget(bwWidget::WIDGET_TYPE_LABEL, "bwLabel", width_hint, height_hint),
      text(std::move(text))
{
  initialize();
}

void bwLabel::draw(bwStyle &style)
{
  bwPainter painter;
  style.setWidgetStyle(*this);

  painter.setContentMask(rectangle);
  painter.setActiveColor(text_color);
  painter.drawTextAndIcon(text, icon, rectangle, TEXT_ALIGN_LEFT, style.dpi_fac);
}

void bwLabel::registerProperties()
{
  style_properties.addColor("color", text_color);
}

const std::string *bwLabel::getLabel() const
{
  return &text;
}

bwLabel &bwLabel::setIcon(const bwIconInterface &icon_interface)
{
  icon = &icon_interface;
  return *this;
}
