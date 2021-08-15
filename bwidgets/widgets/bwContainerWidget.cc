#include "bwContainerWidget.h"

namespace bWidgets {

bwContainerWidget::bwContainerWidget(const bwScreenGraph::ContainerNode& node,
                                     std::optional<unsigned int> width_hint,
                                     std::optional<unsigned int> height_hint)
    : bwWidget(width_hint, height_hint), node(node)
{
  initialize();
}

auto bwContainerWidget::getMaskRectangle() -> bwRectanglePixel
{
  bwRectanglePixel maskrect = rectangle;

  if (base_style.isBorderVisible()) {
    /* With a visible border, we have to exclude the border size from the mask, so that content
     * appears clipped by the border. */
    maskrect.resize(-1);
  }

  return maskrect;
}

auto bwContainerWidget::childrenVisible() const -> bool
{
  /* By default, all children are visible, can be overriden if the container widget supports hiding
   * children. */
  return true;
}

void bwContainerWidget::registerProperties()
{
  base_style.registerProperties(style_properties);
}

}  // namespace bWidgets
