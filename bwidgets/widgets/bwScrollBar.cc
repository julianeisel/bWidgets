#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"

#include "bwScrollBar.h"

using namespace bWidgets;

bwScrollBar::bwScrollBar(unsigned int width_hint, unsigned int height_hint)
    : bwAbstractButton("", Type::SCROLL_BAR, "bwScrollBar", width_hint, height_hint)
{
}

static bwRectanglePixel getInnerRect(bwScrollBar& scrollbar)
{
  bwRectanglePixel rect_inner{scrollbar.rectangle};

  rect_inner.ymax -= (scrollbar.ratio * scrollbar.scroll_offset);
  rect_inner.ymin = rect_inner.ymax - (scrollbar.ratio * scrollbar.rectangle.height());

  return rect_inner;
}

void bwScrollBar::draw(bwStyle& style)
{
  style.setWidgetStyle(*this);

  bwRectanglePixel rect_inner{getInnerRect(*this)};

  /* TODO maybe a wrapper could ensure old style is unchanged after drawing (by resetting it) */
  const char old_shade_top = base_style.shade_top;
  const char old_shade_bottom = base_style.shade_bottom;
  const bwGradient gradient_outer{base_style.backgroundColor(),
                                  base_style.shadeBottom(),
                                  base_style.shadeTop(),
                                  bwGradient::Direction::LEFT_RIGHT};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(
      base_style, style, rectangle, gradient_outer, base_style.corner_radius);

  if (base_style.shadeTop() == base_style.shadeBottom()) {
    // no shading, skip
  }
  else if (base_style.shadeTop() > base_style.shadeBottom()) {
    base_style.shade_top += 20;
  }
  else {
    base_style.shade_bottom += 20;
  }

  const bwGradient gradient_inner{base_style.decorationColor(),
                                  base_style.shadeTop(),
                                  base_style.shadeBottom(),
                                  bwGradient::Direction::LEFT_RIGHT};
  painter.drawRoundboxWidgetBase(
      base_style, style, rect_inner, gradient_inner, base_style.corner_radius);

  base_style.shade_top = old_shade_top;
  base_style.shade_bottom = old_shade_bottom;
}

// ------------------ Handling ------------------

class bwScrollBarHandler : public bwAbstractButtonHandler {
 public:
  bwScrollBarHandler(bwScrollBar& scrollbar);
  ~bwScrollBarHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseClick(bwMouseButtonEvent&) override;
  void onMouseDrag(bwMouseButtonDragEvent&) override;

 private:
  bwScrollBar& scrollbar;

  constexpr static float SCROLL_JUMP_FAC = 0.8f;

  int mouse_press_scroll_offset = 0;  // scroll_offset from last onMousePress() call

  void setScrollOffset(int);
};

bwScrollBarHandler::bwScrollBarHandler(bwScrollBar& scrollbar)
    : bwAbstractButtonHandler(scrollbar), scrollbar(scrollbar)
{
}

bwPtr<bwScreenGraph::EventHandler> bwScrollBar::createHandler()
{
  return bwPtr_new<bwScrollBarHandler>(*this);
}

void bwScrollBarHandler::onMousePress(bwMouseButtonEvent& event)
{
  bwAbstractButtonHandler::onMousePress(event);
  mouse_press_scroll_offset = scrollbar.scroll_offset;
  event.swallow();
}

void bwScrollBarHandler::onMouseClick(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::BUTTON_LEFT) {
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
  if (event.button == bwMouseButtonEvent::BUTTON_LEFT) {
    setScrollOffset(mouse_press_scroll_offset - event.drag_distance.y);
    event.swallow();
  }
}

void bwScrollBarHandler::setScrollOffset(int value)
{
  scrollbar.scroll_offset = value;
  apply();
}
