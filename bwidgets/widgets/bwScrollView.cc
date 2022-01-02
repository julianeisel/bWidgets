#include <cassert>
#include <cmath>
#include <iostream>

#include "bwEvent.h"
#include "bwPainter.h"
#include "bwRange.h"
#include "bwStyle.h"

#include "screen_graph/Builder.h"
#include "screen_graph/Drawer.h"
#include "screen_graph/Node.h"

#include "bwScrollBar.h"

#include "bwScrollView.h"

namespace bWidgets {

bwScrollView::bwScrollView(bwScreenGraph::ContainerNode& node,
                           std::string_view identifier,
                           unsigned int _width,
                           unsigned int _height)
    : bwContainerWidget(node, _width, _height), identifier(identifier)
{
  auto scrollbar = WidgetNew<bwScrollBar>(17, _height);
  scrollbar_node = std::make_unique<bwScreenGraph::WidgetNode>();

  bwScreenGraph::Builder::setWidget(*scrollbar_node, std::move(scrollbar));
}

auto bwScrollView::getTypeIdentifier() const -> std::string_view
{
  return "bwScrollView";
}

auto bwScrollView::getVerticalScrollBar() const -> bwScrollBar&
{
  return static_cast<bwScrollBar&>(*scrollbar_node->Widget());
}

auto bwScrollView::getVerticalScrollbarRect(const bwStyle& style) const -> bwRectanglePixel
{
  bwRectanglePixel scroll_rectangle{rectangle};
  /* TODO hardcoded padding */
  const int padding = 4 * (int)style.dpi_fac;

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
  scrollbar.ratio_ = (rectangle.height() - 2) / float(getContentRect().height());
  scrollbar.setScrollOffset(getVerticalScroll());

  bwScreenGraph::Drawer::drawSubtree(*scrollbar_node, style);
}

void bwScrollView::draw(bwStyle& style)
{
  setContentRect(node.ContentRectangle());

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

auto bwScrollView::matches(const bwWidget& other) const -> std::optional<bool>
{
  const bwScrollView* other_scroll_view = widget_cast<bwScrollView>(other);

  return identifier == other_scroll_view->identifier &&
         scrollbar_node->matches(*other_scroll_view->scrollbar_node);
}

void bwScrollView::validizeScrollValues()
{
  assert(isScrollable());

  int scroll_value = getVerticalScroll();
  bwRange<int>::clampValue(scroll_value, 0, getContentRect().height() - node.Rectangle().height());
  setVerticalScroll(scroll_value);
}

auto bwScrollView::getScrollOffsetY() const -> int
{
  return getVerticalScroll();
}

auto bwScrollView::getContentBounds(float interface_scale) const -> bwRectanglePixel
{
  bwRectanglePixel bounds{rectangle};
  if (isScrollable()) {
    bounds.xmax -= getScrollbarWidth(interface_scale);
  }
  return bounds;
}

auto bwScrollView::isScrollable() const -> bool
{
  return (getContentRect().height() > node.Rectangle().height()) || (getVerticalScroll() != 0);
}

auto bwScrollView::getScrollbarWidth(float interface_scale) -> int
{
  return std::round(SCROLL_BAR_SIZE * interface_scale);
}

// ------------------ State ------------------

struct bwScrollViewState : public bwWidgetStateAlwaysPersistent {
  /** Last known content rectangle, updated before drawing. */
  bwRectanglePixel content_rect;
  /** Current vertical scroll value. */
  int vert_scroll{0};
};

auto bwScrollView::createState() const -> std::unique_ptr<bwWidgetState>
{
  return std::make_unique<bwScrollViewState>();
}

auto bwScrollView::state() const -> bwScrollViewState&
{
  return getState<bwScrollViewState>();
}

auto bwScrollView::getContentRect() const -> bwRectanglePixel
{
  return state().content_rect;
}
void bwScrollView::setContentRect(bwRectanglePixel rect)
{
  state().content_rect = rect;
}

auto bwScrollView::getVerticalScroll() const -> int
{
  return state().vert_scroll;
}

void bwScrollView::setVerticalScroll(int vert_scroll)
{
  state().vert_scroll = vert_scroll;
}

// ------------------ Handling ------------------

class bwScrollViewHandler : public bwScreenGraph::WidgetEventHandler<bwScrollView> {
 public:
  bwScrollViewHandler(bwScreenGraph::Node& node);
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

  auto isEventInsideScrollbar(const class bwEvent& event) const -> bool;

  void setScrollValue(int value);

 private:
  constexpr static int SCROLL_STEP_SIZE = 40;

  bool was_inside_scrollbar{false};
};

bwScrollViewHandler::bwScrollViewHandler(bwScreenGraph::Node& node)
    : bwScreenGraph::WidgetEventHandler<bwScrollView>(node)
{
}

auto bwScrollView::createHandler(bwScreenGraph::Node& node) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwScrollViewHandler>(node);
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
static auto forwardEventToScrollbarIfInside(const bwScrollViewHandler& scrollview_handler,
                                            bwScreenGraph::Node& scrollbar_node,
                                            const class bwEvent& event,
                                            HandlerFunc<_Args&&...> handler_func,
                                            _Args&&... __args) -> bool
{
  if (scrollview_handler.isEventInsideScrollbar(event)) {
    forwardEventToNode<_Args&&...>(scrollbar_node, handler_func, std::forward<_Args>(__args)...);
    return true;
  }

  return false;
}

void bwScrollViewHandler::onMouseWheel(bwMouseWheelEvent& event)
{
  if (!Widget().isScrollable()) {
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

  setScrollValue(Widget().getVerticalScroll() + (direction_fac * SCROLL_STEP_SIZE));

  event.swallow();
}

auto bwScrollViewHandler::isEventInsideScrollbar(const bwEvent& event) const -> bool
{
  return Widget().isScrollable() && Widget().scrollbar_node->Rectangle().isCoordinateInside(
                                        event.location.x, event.location.y);
}

void bwScrollViewHandler::onScrollbarMouseEnter(bwEvent& event) const
{
  forwardEventToNode<bwEvent&>(*Widget().scrollbar_node, &EventHandler::onMouseEnter, event);
}

void bwScrollViewHandler::onScrollbarMouseLeave(bwEvent& event) const
{
  forwardEventToNode<bwEvent&>(*Widget().scrollbar_node, &EventHandler::onMouseLeave, event);
}

void bwScrollViewHandler::onMouseMove(bwEvent& event)
{
  forwardEventToScrollbarIfInside<bwEvent&>(
      *this, *Widget().scrollbar_node, event, &EventHandler::onMouseMove, event);

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
          *this, *Widget().scrollbar_node, event, &EventHandler::onMouseDrag, event)) {
    setScrollValue(Widget().getVerticalScrollBar().getScrollOffset());
    event.swallow();
  }
}

void bwScrollViewHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
          *this, *Widget().scrollbar_node, event, &EventHandler::onMousePress, event)) {
    event.swallow();
  }
}

void bwScrollViewHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
          *this, *Widget().scrollbar_node, event, &EventHandler::onMouseRelease, event)) {
    event.swallow();
  }
}

void bwScrollViewHandler::onMouseClick(bwMouseButtonEvent& event)
{
  if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
          *this, *Widget().scrollbar_node, event, &EventHandler::onMouseClick, event)) {
    setScrollValue(Widget().getVerticalScrollBar().getScrollOffset());
    event.swallow();
  }
}

void bwScrollViewHandler::setScrollValue(int value)
{
  assert(Widget().isScrollable());

  Widget().setVerticalScroll(value);
  Widget().validizeScrollValues();
}

}  // namespace bWidgets
