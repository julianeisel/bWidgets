#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

namespace bwScreenGraph {
class ContainerNode;
}

class bwContainerWidget : public bwWidget {
 public:
  virtual auto getMaskRectangle() -> bwRectanglePixel;
  virtual auto childrenVisible() const -> bool;

  void registerProperties() override;

 protected:
  bwContainerWidget(const bwScreenGraph::ContainerNode& node,
                    std::optional<unsigned int> width_hint = std::nullopt,
                    std::optional<unsigned int> height_hint = std::nullopt);

  /** Reference to node owning this widget, for additional info queries. */
  const bwScreenGraph::ContainerNode& node;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

}  // namespace bWidgets
