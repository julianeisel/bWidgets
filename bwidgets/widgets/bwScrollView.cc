#include <cmath>

#include "bwEvent.h"
#include "bwPainter.h"
#include "bwRange.h"
#include "bwStyle.h"

#include "screen_graph/Builder.h"
#include "screen_graph/Node.h"

#include "bwScrollBar.h"

#include "bwScrollView.h"

namespace bWidgets {

bwScrollView::bwScrollView(bwScreenGraph::ContainerNode& node,
                           unsigned int _width,
                           unsigned int _height)
    : bwContainerWidget(node, bwWidget::Type::SCROLL_VIEW, "bwScrollView", _width, _height)
{
  auto scrollbar = bwPtr_new<bwScrollBar>(17, _height);
  scrollbar_node = bwPtr_new<bwScreenGraph::WidgetNode>();

  bwScreenGraph::Builder::setWidget(*scrollbar_node, std::move(scrollbar));
}

bwScrollBar& bwScrollView::getVerticalScrollBar() const
{
  return static_cast<bwScrollBar&>(*scrollbar_node->Widget());
}

bwRectanglePixel bwScrollView::getVerticalScrollbarRect(const bwStyle& style) const
{
  bwRectanglePixel scroll_rectangle{rectangle};
  /* TODO hardcoded padding */
  const unsigned int padding = 4u * (unsigned int)style.dpi_fac;

  scroll_rectangle.xmin = scroll_rectangle.xmax - bwScrollView::getScrollbarWidth(style.dpi_fac) -
                          padding;
  scroll_rectangle.resize(-padding);

  return scroll_rectangle;
}

void bwScrollView::drawScrollBars(bwStyle& style)
{
  bwScrollBar& scrollbar = getVerticalScrollBar();

  validizeScrollValues();

  scrollbar.rectangle = getVerticalScrollbarRect(style);
  scrollbar.ratio = (rectangle.height() - 2) / float(node.ContentRectangle().height());
  scrollbar.scroll_offset = vert_scroll;

  scrollbar.draw(style);
}

void bwScrollView::draw(bwStyle& style)
{
  style.setWidgetStyle(*this);

  bwPainter painter;

  painter.active_drawtype = bwPainter::DrawType::FILLED;
  painter.setActiveColor(base_style.background_color);
  painter.drawRectangle(rectangle);

  if (base_style.isBorderVisible()) {
    painter.active_drawtype = bwPainter::DrawType::OUTLINE;
    painter.setActiveColor(base_style.border_color);
    painter.drawRectangle(rectangle);
  }
  if (isScrollable()) {
    drawScrollBars(style);
  }
}

void bwScrollView::validizeScrollValues()
{
  assert(isScrollable());

  bwRange<int>::clampValue(
      vert_scroll, 0, node.ContentRectangle().height() - node.Rectangle().height());
}

int bwScrollView::getScrollOffsetY() const
{
  return vert_scroll;
}

bwRectanglePixel bwScrollView::getContentBounds(float interface_scale) const
{
  bwRectanglePixel bounds{rectangle};
  if (isScrollable()) {
    bounds.xmax -= getScrollbarWidth(interface_scale);
  }
  return bounds;
}

bool bwScrollView::isScrollable() const
{
  return (node.ContentRectangle().height() > node.Rectangle().height()) || (vert_scroll != 0);
}

int bwScrollView::getScrollbarWidth(float interface_scale)
{
  return std::round(SCROLL_BAR_SIZE * interface_scale);
}

// ------------------ Handling ------------------

class bwScrollViewHandler : public bwScreenGraph::EventHandler {
 public:
  bwScrollViewHandler(bwScrollView& scrollview);
  ~bwScrollViewHandler() = default;

  void onMouseMove(bwEvent&) override;
  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent& event) override;
  void onMouseRelease(bwMouseButtonEvent& event) override;
  void onMouseClick(bwMouseButtonEvent& event) override;
  void onMouseDrag(bwMouseButtonDragEvent& event) override;
  void onMouseWheel(bwMouseWheelEvent& event) override;

  void onScrollbarMouseEnter(bwEvent& event) const;
  void onScrollbarMouseLeave(bwEvent& event) const;

  bool isEventInsideScrollbar(const class bwEvent& event) const;

  void setScrollValue(int value);

 private:
  bwScrollView& scrollview;
  constexpr static int SCROLL_STEP_SIZE = 40;

  bool was_inside_scrollbar{false};
};

bwScrollViewHandler::bwScrollViewHandler(bwScrollView& panel) : scrollview(panel)
{
}

bwPtr<bwScreenGraph::EventHandler> bwScrollView::createHandler()
{
  return bwPtr_new<bwScrollViewHandler>(*this);
}

template<typename... _Args> using HandlerFunc = void (bwScreenGraph::EventHandler::*)(_Args&&...);

/* Could turn this into a general utility to forward events to different widgets. */
template<typename... _Args>
static void forwardEventToNode(bwScreenGraph::Node& to_node,
                               HandlerFunc<_Args&&...> handler_func,
                               _Args&&... __args)
{
  (to_node.eventHandler()->*handler_func)(std::forward<_Args>(__args)...);
}

template<typename... _Args>
static bool forwardEventToScrollbarIfInside(const bwScrollViewHandler& scrollview_handler,
                                            bwScreenGraph::Node& scrollbar_node,
                                            const class bwEvent& event,
                                            HandlerFunc<_Args&&...> handler_func,
                                            _Args&&... __args)
{
  if (scrollview_handler.isEventInsideScrollbar(event)) {
    forwardEventToNode<_Args&&...>(scrollbar_node, handler_func, std::forward<_Args>(__args)...);
    return true;
  }

  return false;
}

void bwScrollViewHandler::onMouseWheel(bwMouseWheelEvent& event)
{
  if (!scrollview.isScrollable()) {
    return;
  }

  char direction_fac = 0;

  switch (event.getDirection()) {
    case bwMouseWheelEvent::Direction::UP:
      direction_fac = -1;
      break;
    case bwMouseWheelEvent::Direction::DOWN:
      direction_fac = 1;
      break;
  }

  setScrollValue(scrollview.vert_scroll + (direction_fac * SCROLL_STEP_SIZE));

  event.swallow();
}

bool bwScrollViewHandler::isEventInsideScrollbar(const bwEvent& event) const
{
  return scrollview.isScrollable() && scrollview.scrollbar_node->Rectangle().isCoordinateInside(
                                          event.location.x, event.location.y);
}

void bwScrollViewHandler::onScrollbarMouseEnter(bwEvent& event) const
{
  forwardEventToNode<bwEvent&>(*scrollview.scrollbar_node, &EventHandler::onMouseEnter, event);
}

void bwScrollViewHandler::onScrollbarMouseLeave(bwEvent& event) const
{
  forwardEventToNode<bwEvent&>(*scrollview.scrollbar_node, &EventHandler::onMouseLeave, event);
}

void bwScrollViewHandler::onMouseMove(bwEvent& event)
{
  forwardEventToScrollbarIfInside<bwEvent&>(
      *this, *scrollview.scrollbar_node, event, &EventHandler::onMouseMove, event);

  if (was_inside_scrollbar && !isEventInsideScrollbar(event)) {
    onScrollbarMouseLeave(event);
    was_inside_scrollbar = false;
  }
  else if (!was_inside_scrollbar && isEventInsideScrollbar(event)) {
    onScrollbarMouseEnter(event);
    was_inside_scrollbar = true;
  }
}

void bwScrollViewHandler::onMouseEnter(bwEvent& event)
{
  if (!was_inside_scrollbar && isEventInsideScrollbar(event)) {
    onScrollbarMouseEnter(event);
    was_inside_scrollbar = true;
  }
}

void bwScrollViewHandler::onMouseLeave(bwEvent& event)
{
  if (was_inside_scrollbar && !isEventInsideScrollbar(event)) {
    onScrollbarMouseLeave(event);
    was_inside_scrollbar = false;
  }
}

void bwScrollViewHandler::onMouseDrag(bwMouseButtonDragEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonDragEvent&>(
          *this, *scrollview.scrollbar_node, event, &EventHandler::onMouseDrag, event)) {
    setScrollValue(scrollview.getVerticalScrollBar().scroll_offset);
  }
}

void bwScrollViewHandler::onMousePress(bwMouseButtonEvent& event)
{
  forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
      *this, *scrollview.scrollbar_node, event, &EventHandler::onMousePress, event);
}

void bwScrollViewHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
      *this, *scrollview.scrollbar_node, event, &EventHandler::onMouseRelease, event);
}

void bwScrollViewHandler::onMouseClick(bwMouseButtonEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
          *this, *scrollview.scrollbar_node, event, &EventHandler::onMouseClick, event)) {
    setScrollValue(scrollview.getVerticalScrollBar().scroll_offset);
  }
}

void bwScrollViewHandler::setScrollValue(int value)
{
  assert(scrollview.isScrollable());

  scrollview.vert_scroll = value;
  scrollview.validizeScrollValues();
}

}  // namespace bWidgets
