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
                    const Type type,
                    const std::string& identifier,
                    const unsigned int width_hint = 0,
                    const unsigned int height_hint = 0);

  /** Reference to node owning this widget, for additional info queries. */
  const bwScreenGraph::ContainerNode& node;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

}  // namespace bWidgets
