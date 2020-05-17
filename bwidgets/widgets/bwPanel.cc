#include <cassert>
#include <cmath>
#include <iostream>

#include "bwEvent.h"
#include "bwPainter.h"
#include "bwPanel.h"
#include "bwStyle.h"

namespace bWidgets {

bwPanel::bwPanel(const bwScreenGraph::ContainerNode& node,
                 std::string label,
                 unsigned int header_height_hint)
    : bwContainerWidget(node, "bwPanel", 0, header_height_hint),
      header_height(header_height_hint),
      label(std::move(label))
{
  initialize();
}

void bwPanel::draw(bwStyle& style)
{
  style.setWidgetStyle(*this);

  const bwGradient gradient{
      base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(base_style, style, rectangle, gradient, base_style.corner_radius);

  drawHeader(style);
}

void bwPanel::registerProperties()
{
  bwContainerWidget::registerProperties();
  style_properties.addBool("draw-separator", draw_separator);
}

const std::string* bwPanel::getLabel() const
{
  return &label;
}

bool bwPanel::childrenVisible() const
{
  return panel_state == State::OPEN;
}

bool bwPanel::isCoordinateInsideHeader(const bwPoint& point) const
{
  const bwRectanglePixel header_rect = getHeaderRectangle();
  return header_rect.isCoordinateInside(point.x, point.y);
}

unsigned int bwPanel::getHeaderHeightHint() const
{
  return height_hint;
}

// TODO GPL!
static void panel_draw_drag_dots(bwPainter& painter,
                                 const bwRectanglePixel& rectangle,
                                 const bwWidgetBaseStyle& base_style)
{
  const int px = 1.0f;  // TODO Equivalent to U.pixelsize.
  const int px_zoom = std::max((int)std::round(rectangle.height() / 22.0f), 1);
  const float tint = 84.0f / 255.0f;

  const int padding = std::max((int)std::round(rectangle.height() / 3.0f), px);
  const int dot_margin = std::max((int)std::round(px_zoom * 2.0f), px);
  const int dot_size = std::max((int)std::round((rectangle.height() / 8.0f) - px), px);

  bwColor dot_color = base_style.backgroundColor();
  bwColor shadow_color = base_style.backgroundColor();

  dot_color.shade(tint);
  shadow_color.shade(-tint);

  painter.active_drawtype = bwPainter::DrawType::FILLED;
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 2; row++) {
      const bwPoint pos{((float)rectangle.xmin + padding) + (col * (dot_size + dot_margin)),
                        ((float)rectangle.ymin + padding) + (row * (dot_size + dot_margin))};
      bwRectanglePixel dot_rect{
          (int)pos.x - dot_size, (int)pos.x, (int)pos.y, (int)pos.y + dot_size};
      bwRectanglePixel shadow_rect{dot_rect};

      shadow_rect.ymin -= dot_size;
      shadow_rect.ymax -= dot_size;

      painter.setActiveColor(shadow_color);
      painter.drawRectangle(shadow_rect);
      painter.setActiveColor(dot_color);
      painter.drawRectangle(dot_rect);
    }
  }
}

void bwPanel::drawHeader(bwStyle& style) const
{
  bwRectanglePixel header_rect = getHeaderRectangle();
  bwRectanglePixel text_rect = header_rect;
  bwRectangle<float> drag_rect = header_rect;
  bwRectangle<float> triangle_rect =
      header_rect;  // Using float makes the triangle slightly bigger, matching Blender.
  bwPainter painter;

  // TODO How the panel header will eventually be handled isn't entirely sure
  // yet. It may become its own widget, or the layout adds the needed widgets
  // for it (e.g. bwLabel for the panel label). For now, let bwPanel draw the
  // label itself.

  if (draw_separator) {
    painter.setActiveColor(bwColor(0.0f, 0.5f));
    painter.drawLine(bwPoint(header_rect.xmin, header_rect.ymax),
                     bwPoint(header_rect.xmax, header_rect.ymax));
    painter.setActiveColor(bwColor(1.0f, 0.25f));
    painter.drawLine(bwPoint(header_rect.xmin, header_rect.ymax - 1),
                     bwPoint(header_rect.xmax, header_rect.ymax - 1));
  }

  text_rect.xmin += (20 * style.dpi_fac) + 5;
  text_rect.xmin -= 10;  // XXX Demo app adds 10px when drawing text.
  painter.setActiveColor(base_style.textColor());
  painter.drawText(label, text_rect, base_style.text_alignment);

  drag_rect.xmax -= 5.0f;
  drag_rect.xmin = drag_rect.xmax - drag_rect.height();
  drag_rect.scale(0.7f);
  panel_draw_drag_dots(painter, drag_rect, base_style);

  triangle_rect.xmin += 5;
  triangle_rect.xmax = triangle_rect.xmin + triangle_rect.height();
  triangle_rect.scale(0.35f);
  painter.active_drawtype = bwPainter::DrawType::FILLED;
  painter.setActiveColor(base_style.textColor());
  painter.drawTriangle(triangle_rect,
                       (panel_state == State::OPEN) ? Direction::DOWN : Direction::RIGHT);
}

bwRectanglePixel bwPanel::getHeaderRectangle() const
{
  bwRectanglePixel header_rect{rectangle};
  header_rect.ymin = header_rect.ymax - header_height;
  return header_rect;
}

// ------------------ Handling ------------------

class bwPanelHandler : public bwScreenGraph::EventHandler {
 public:
  bwPanelHandler(bwPanel& panel);
  ~bwPanelHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;

 private:
  bwPanel& panel;
};

bwPanelHandler::bwPanelHandler(bwPanel& panel) : panel(panel)
{
}

bwPtr<bwScreenGraph::EventHandler> bwPanel::createHandler()
{
  return bwPtr_new<bwPanelHandler>(*this);
}

void bwPanelHandler::onMousePress(bwMouseButtonEvent& event)
{
  if ((event.button != bwMouseButtonEvent::BUTTON_LEFT) ||
      !panel.isCoordinateInsideHeader(event.location)) {
    // Skip
  }
  else if (panel.panel_state == bwPanel::State::CLOSED) {
    panel.panel_state = bwPanel::State::OPEN;
    event.swallow();
  }
  else if (panel.panel_state == bwPanel::State::OPEN) {
    panel.panel_state = bwPanel::State::CLOSED;
    event.swallow();
  }
  else {
    assert(0);
  }
}

}  // namespace bWidgets
