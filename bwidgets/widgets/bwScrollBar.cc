#include <iostream>
#include <memory>

#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"
#include "screen_graph/Node.h"

#include "bwScrollBar.h"

namespace bWidgets {

bwScrollBar::bwScrollBar(unsigned int width_hint, unsigned int height_hint)
    : bwAbstractButton("", width_hint, height_hint)
{
}

auto bwScrollBar::getTypeIdentifier() const -> std::string_view
{
  return "bwScrollBar";
}

auto bwScrollBar::getInnerRect(bwScrollBar& scrollbar) -> bwRectanglePixel
{
  bwRectanglePixel rect_inner{scrollbar.rectangle};

  rect_inner.ymax -= (scrollbar.ratio_ * scrollbar.getScrollOffset());
  rect_inner.ymin = rect_inner.ymax - (scrollbar.ratio_ * scrollbar.rectangle.height());

  return rect_inner;
}

void bwScrollBar::draw(bwStyle& style)
{
  bwRectanglePixel rect_inner{getInnerRect(*this)};

  bwWidgetBaseStyle tmp_base_style = base_style;
  const bwGradient gradient_outer{tmp_base_style.backgroundColor(),
                                  tmp_base_style.shadeBottom(),
                                  tmp_base_style.shadeTop(),
                                  bwGradient::Direction::LEFT_RIGHT};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(
      tmp_base_style, style, rectangle, gradient_outer, tmp_base_style.corner_radius);

  if (tmp_base_style.shadeTop() == tmp_base_style.shadeBottom()) {
    // no shading, skip
  }
  else if (tmp_base_style.shadeTop() > tmp_base_style.shadeBottom()) {
    tmp_base_style.shade_top += 20;
  }
  else {
    tmp_base_style.shade_bottom += 20;
  }

  const bwGradient gradient_inner{tmp_base_style.decorationColor(),
                                  tmp_base_style.shadeTop(),
                                  tmp_base_style.shadeBottom(),
                                  bwGradient::Direction::LEFT_RIGHT};
  painter.drawRoundboxWidgetBase(
      tmp_base_style, style, rect_inner, gradient_inner, tmp_base_style.corner_radius);
}

auto bwScrollBar::matches(const bwWidget& /*other*/) const -> bool
{
  /* XXX There's no data to compare here. */
  return true;
}

auto bwScrollBar::alwaysPersistent() const -> bool
{
  return true;
}

// ------------------ State ------------------

struct bwScrollBarState : public bwWidgetState {
  int scroll_offset = 0;
};

void bwScrollBar::createState()
{
  state_ = std::make_unique<bwScrollBarState>();
}

int bwScrollBar::getScrollOffset()
{
  return getState<bwScrollBarState>().scroll_offset;
}

void bwScrollBar::setScrollOffset(int scroll_offset)
{
  getState<bwScrollBarState>().scroll_offset = scroll_offset;
}

// ------------------ Handling ------------------

class bwScrollBarHandler : public bwAbstractButtonHandler<bwScrollBar> {
 public:
  bwScrollBarHandler(bwScreenGraph::Node& node);
  ~bwScrollBarHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseClick(bwMouseButtonEvent&) override;
  void onMouseDrag(bwMouseButtonDragEvent&) override;

 private:
  constexpr static float SCROLL_JUMP_FAC = 0.8f;

  int mouse_press_scroll_offset = 0;  // scroll_offset from last onMousePress() call

  void setScrollOffset(int);
};

bwScrollBarHandler::bwScrollBarHandler(bwScreenGraph::Node& node) : bwAbstractButtonHandler(node)
{
}

auto bwScrollBar::createHandler(bwScreenGraph::Node& node) const
    -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwScrollBarHandler>(node);
}

void bwScrollBarHandler::onMousePress(bwMouseButtonEvent& event)
{
  bwAbstractButtonHandler::onMousePress(event);
  mouse_press_scroll_offset = Widget().getScrollOffset();
  event.swallow();
}

void bwScrollBarHandler::onMouseClick(bwMouseButtonEvent& event)
{
  bwScrollBar& scrollbar = Widget();
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    bwRectanglePixel rect_inner{scrollbar.getInnerRect(scrollbar)};

    if (event.location.y > rect_inner.ymax) {
      setScrollOffset(scrollbar.getScrollOffset() -
                      (scrollbar.rectangle.height() * SCROLL_JUMP_FAC));
    }
    else if (event.location.y < rect_inner.ymin) {
      setScrollOffset(scrollbar.getScrollOffset() +
                      (scrollbar.rectangle.height() * SCROLL_JUMP_FAC));
    }
  }

  event.swallow();
}

void bwScrollBarHandler::onMouseDrag(bwMouseButtonDragEvent& event)
{
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    setScrollOffset(mouse_press_scroll_offset - event.drag_distance.y);
    event.swallow();
  }
}

void bwScrollBarHandler::setScrollOffset(int value)
{
  Widget().setScrollOffset(value);
  apply();
}

}  // namespace bWidgets
