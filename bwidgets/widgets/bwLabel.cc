#include <cmath>

#include "bwPainter.h"
#include "bwStyle.h"

#include "bwLabel.h"

namespace bWidgets {

bwLabel::bwLabel(std::string text,
                 bwOptional<unsigned int> width_hint,
                 bwOptional<unsigned int> height_hint)
    : bwWidget("bwLabel", width_hint, height_hint), text(std::move(text))
{
  initialize();
}

void bwLabel::draw(bwStyle& style)
{
  bwPainter painter;
  style.setWidgetStyle(*this);

  painter.setContentMask(rectangle);
  painter.setActiveColor(text_color);
  painter.drawTextAndIcon(text, icon, rectangle, TextAlignment::LEFT, style.dpi_fac);
}

void bwLabel::registerProperties()
{
  style_properties.addColor("color", text_color);
}

const std::string* bwLabel::getLabel() const
{
  return &text;
}

std::unique_ptr<bwScreenGraph::EventHandler> bwLabel::createHandler()
{
  return nullptr;
}

bwLabel& bwLabel::setIcon(const bwIconInterface& icon_interface)
{
  icon = &icon_interface;
  return *this;
}

}  // namespace bWidgets
