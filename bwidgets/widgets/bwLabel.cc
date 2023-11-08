#include <cmath>

#include "bwPainter.h"
#include "bwStyle.h"

#include "bwLabel.h"

namespace bWidgets {

bwLabel::bwLabel(std::string text,
                 std::optional<unsigned int> width_hint,
                 std::optional<unsigned int> height_hint)
    : bwWidget(width_hint, height_hint), text(std::move(text))
{
  initialize();
}

auto bwLabel::getTypeIdentifier() const -> std::string_view
{
  return "bwLabel";
}

void bwLabel::draw(bwStyle& style)
{
  bwPainter painter;

  painter.setContentMask(rectangle);
  painter.setActiveColor(text_color);
  painter.drawTextAndIcon(text, icon, rectangle, TextAlignment::LEFT, style.dpi_fac);
}

auto bwLabel::matches(const bwWidget&) const -> bool
{
  /* Labels don't have real state that needs to be kept over updates, so don't match labels. */
  return false;
}

void bwLabel::registerProperties()
{
  style_properties.addColor("color", text_color);
}

auto bwLabel::getLabel() const -> const std::string*
{
  return &text;
}

auto bwLabel::setLabel(const std::string& label) -> bwLabel&
{
  text = label;
  return *this;
}

auto bwLabel::createHandler(bwScreenGraph::Node& /*node*/) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return nullptr;
}

auto bwLabel::setIcon(const bwIconInterface& icon_interface) -> bwLabel&
{
  icon = &icon_interface;
  return *this;
}

}  // namespace bWidgets
