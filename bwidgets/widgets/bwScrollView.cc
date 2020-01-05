#include <cmath>

#include "bwEvent.h"
#include "bwPainter.h"
#include "bwRange.h"
#include "bwStyle.h"

#include "screen_graph/Builder.h"
#include "screen_graph/Node.h"

#include "bwScrollBar.h"

#include "bwScrollView.h"

using namespace bWidgets;

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
static bool forwardEventToScrollbarIfInside(const bwScrollView& scrollview,
                                            bwScreenGraph::Node& scrollbar_node,
                                            const class bwEvent& event,
                                            HandlerFunc<_Args&&...> handler_func,
                                            _Args&&... __args)
{
  if (scrollview.isEventInsideScrollbar(event)) {
    forwardEventToNode<_Args&&...>(scrollbar_node, handler_func, std::forward<_Args>(__args)...);
    return true;
  }

  return false;
}

void bwScrollView::onMouseWheel(bwMouseWheelEvent& event)
{
  if (!isScrollable()) {
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

  setScrollValue(vert_scroll + (direction_fac * SCROLL_STEP_SIZE));

  event.swallow();
}

bool bwScrollView::isEventInsideScrollbar(const bwEvent& event) const
{
  return isScrollable() &&
         scrollbar_node->Rectangle().isCoordinateInside(event.location.x, event.location.y);
}

void bwScrollView::onScrollbarMouseEnter(bwEvent& event) const
{
  forwardEventToNode<bwEvent&>(*scrollbar_node, &EventHandler::onMouseEnter, event);
}

void bwScrollView::onScrollbarMouseLeave(bwEvent& event) const
{
  forwardEventToNode<bwEvent&>(*scrollbar_node, &EventHandler::onMouseLeave, event);
}

void bwScrollView::onMouseMove(bwEvent& event)
{
  forwardEventToScrollbarIfInside<bwEvent&>(
      *this, *scrollbar_node, event, &EventHandler::onMouseMove, event);

  if (was_inside_scrollbar && !isEventInsideScrollbar(event)) {
    onScrollbarMouseLeave(event);
    was_inside_scrollbar = false;
  }
  else if (!was_inside_scrollbar && isEventInsideScrollbar(event)) {
    onScrollbarMouseEnter(event);
    was_inside_scrollbar = true;
  }
}

void bwScrollView::onMouseEnter(bwEvent& event)
{
  if (!was_inside_scrollbar && isEventInsideScrollbar(event)) {
    onScrollbarMouseEnter(event);
    was_inside_scrollbar = true;
  }
}

void bwScrollView::onMouseLeave(bwEvent& event)
{
  if (was_inside_scrollbar && !isEventInsideScrollbar(event)) {
    onScrollbarMouseLeave(event);
    was_inside_scrollbar = false;
  }
}

void bwScrollView::onMouseDrag(bwMouseButtonDragEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonDragEvent&>(
          *this, *scrollbar_node, event, &EventHandler::onMouseDrag, event)) {
    setScrollValue(getVerticalScrollBar().scroll_offset);
  }
}

void bwScrollView::onMousePress(bwMouseButtonEvent& event)
{
  forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
      *this, *scrollbar_node, event, &EventHandler::onMousePress, event);
}

void bwScrollView::onMouseRelease(bwMouseButtonEvent& event)
{
  forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
      *this, *scrollbar_node, event, &EventHandler::onMouseRelease, event);
}

void bwScrollView::onMouseClick(bwMouseButtonEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
          *this, *scrollbar_node, event, &EventHandler::onMouseClick, event)) {
    setScrollValue(getVerticalScrollBar().scroll_offset);
  }
}

void bwScrollView::validizeScrollValues()
{
  assert(isScrollable());

  bwRange<int>::clampValue(
      vert_scroll, 0, node.ContentRectangle().height() - node.Rectangle().height());
}

void bwScrollView::setScrollValue(int value)
{
  assert(isScrollable());

  vert_scroll = value;
  validizeScrollValues();
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
