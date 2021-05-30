#pragma once

#include <cmath>

#include "bwContainerWidget.h"

namespace bWidgets {

namespace bwScreenGraph {
class WidgetNode;
}
class bwScrollBar;

class bwScrollView : public bwContainerWidget {
  friend class bwScrollViewHandler;

 public:
  bwScrollView(bwScreenGraph::ContainerNode& node,
               unsigned int width = 0,
               unsigned int height = 0);

  void draw(bwStyle& style) override;
  auto matches(const bwWidget& other) const -> bool override;

  auto createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  auto getScrollOffsetY() const -> int;
  auto getContentBounds(float interface_scale) const -> bwRectanglePixel;

 private:
  auto getVerticalScrollBar() const -> bwScrollBar&;
  auto getVerticalScrollbarRect(const bwStyle& style) const -> bwRectanglePixel;
  void drawScrollBars(bwStyle& style);
  auto isScrollable() const -> bool;
  void validizeScrollValues();

  static auto getScrollbarWidth(float interface_scale) -> int;

  constexpr static int SCROLL_BAR_SIZE = 17;

  std::unique_ptr<bwScreenGraph::WidgetNode> scrollbar_node;

  int vert_scroll{0};
};

}  // namespace bWidgets
