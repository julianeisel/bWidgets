#pragma once

#include <cmath>

#include "bwContainerWidget.h"

namespace bWidgets {

namespace bwScreenGraph {
class WidgetNode;
}

class bwScrollView : public bwContainerWidget {
 public:
  bwScrollView(bwScreenGraph::ContainerNode& node,
               unsigned int width = 0,
               unsigned int height = 0);

  void draw(bwStyle& style) override;

  void onMouseMove(bwEvent&) override;
  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent& event) override;
  void onMouseRelease(bwMouseButtonEvent& event) override;
  void onMouseClick(bwMouseButtonEvent& event) override;
  void onMouseDrag(bwMouseButtonDragEvent& event) override;
  void onMouseWheel(bwMouseWheelEvent& event) override;

  int getScrollOffsetY() const;
  bwRectanglePixel getContentBounds(float interface_scale) const;
  bool isEventInsideScrollbar(const class bwEvent& event) const;

 private:
  constexpr static int SCROLL_STEP_SIZE = 40;
  constexpr static int SCROLL_BAR_SIZE = 17;

  bwPtr<bwScreenGraph::WidgetNode> scrollbar_node;
  bool was_inside_scrollbar{false};

  int vert_scroll{0};

  void onScrollbarMouseEnter(bwEvent& event) const;
  void onScrollbarMouseLeave(bwEvent& event) const;

  class bwScrollBar& getVerticalScrollBar() const;
  bwRectanglePixel getVerticalScrollbarRect(const bwStyle& style) const;
  void drawScrollBars(bwStyle& style);
  bool isScrollable() const;
  void setScrollValue(int value);
  void validizeScrollValues();
  static int getScrollbarWidth(float interface_scale);
};

}  // namespace bWidgets
