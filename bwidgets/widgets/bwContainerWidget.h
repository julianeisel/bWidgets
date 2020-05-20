#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

namespace bwScreenGraph {
class ContainerNode;
}

class bwContainerWidget : public bwWidget {
 public:
  virtual bwRectanglePixel getMaskRectangle();
  virtual bool childrenVisible() const;

  void registerProperties() override;

 protected:
  bwContainerWidget(const bwScreenGraph::ContainerNode& node,
                    const std::string& identifier,
                    bwOptional<unsigned int> width_hint = nullopt,
                    bwOptional<unsigned int> height_hint = nullopt);

  /** Reference to node owning this widget, for additional info queries. */
  const bwScreenGraph::ContainerNode& node;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

}  // namespace bWidgets
