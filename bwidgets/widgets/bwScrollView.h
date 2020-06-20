#pragma once

#include <cmath>

#include "bwContainerWidget.h"

namespace bWidgets {

namespace bwScreenGraph {
class WidgetNode;
}

class bwScrollView : public bwContainerWidget {
  friend class bwScrollViewHandler;

 public:
  bwScrollView(bwScreenGraph::ContainerNode& node,
               unsigned int width = 0,
               unsigned int height = 0);

  void draw(bwStyle& style) override;

  std::unique_ptr<bwScreenGraph::EventHandler> createHandler() override;

  int getScrollOffsetY() const;
  bwRectanglePixel getContentBounds(float interface_scale) const;

 private:
  constexpr static int SCROLL_BAR_SIZE = 17;

  std::unique_ptr<bwScreenGraph::WidgetNode> scrollbar_node;

  int vert_scroll{0};

  class bwScrollBar& getVerticalScrollBar() const;
  bwRectanglePixel getVerticalScrollbarRect(const bwStyle& style) const;
  void drawScrollBars(bwStyle& style);
  bool isScrollable() const;
  void validizeScrollValues();
  static int getScrollbarWidth(float interface_scale);
};

}  // namespace bWidgets
