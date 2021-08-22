#include <iostream>

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

static auto getInnerRect(bwScrollBar& scrollbar) -> bwRectanglePixel
{
  bwRectanglePixel rect_inner{scrollbar.rectangle};

  rect_inner.ymax -= (scrollbar.ratio * scrollbar.scroll_offset);
  rect_inner.ymin = rect_inner.ymax - (scrollbar.ratio * scrollbar.rectangle.height());

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

auto bwScrollBar::matches(const bwWidget& other) const -> bool
{
  const bwScrollBar* other_scrollbar = widget_cast<bwScrollBar>(other);
  if (!other_scrollbar) {
    return false;
  }

  /* XXX There's no data to compare here. */
  return true;
}

void bwScrollBar::copyState(const bwWidget& from)
{
  bwWidget::copyState(from);

  const bwScrollBar* other_scroll_bar = widget_cast<bwScrollBar>(from);
  if (!other_scroll_bar) {
    return;
  }

  scroll_offset = other_scroll_bar->scroll_offset;
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
  mouse_press_scroll_offset = Widget().scroll_offset;
  event.swallow();
}

void bwScrollBarHandler::onMouseClick(bwMouseButtonEvent& event)
{
  bwScrollBar& scrollbar = Widget();
  if (event.button == bwMouseButtonEvent::Button::LEFT) {
    bwRectanglePixel rect_inner{getInnerRect(scrollbar)};

    if (event.location.y > rect_inner.ymax) {
      setScrollOffset(scrollbar.scroll_offset - (scrollbar.rectangle.height() * SCROLL_JUMP_FAC));
    }
    else if (event.location.y < rect_inner.ymin) {
      setScrollOffset(scrollbar.scroll_offset + (scrollbar.rectangle.height() * SCROLL_JUMP_FAC));
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
  Widget().scroll_offset = value;
  apply();
}

}  // namespace bWidgets
