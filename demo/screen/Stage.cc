/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <cassert>
#include <iostream>
#include <cmath>

// bWidgets lib
#include "bwEvent.h"
#include "bwEventDispatcher.h"
#include "bwPainter.h"
#include "bwPanel.h"
#include "bwRange.h"
#include "bwStyleFlatDark.h"
#include "bwStyleManager.h"
#include "bwWidget.h"
#include "screen_graph/Builder.h"
#include "screen_graph/Iterators.h"

#include "Event.h"
#include "File.h"
#include "Font.h"
#include "GawainPaintEngine.h"
#include "IconMap.h"
#include "Layout.h"
#include "StyleSheet.h"
#include "Window.h"

#include "Stage.h"

using namespace bWidgetsDemo;
using namespace bWidgets;  // Less verbose

bwPtr<bwStyle> Stage::style = nullptr;
bwPtr<StyleSheet> Stage::style_sheet = nullptr;
bwPtr<Font> Stage::font = nullptr;
bwPtr<IconMap> Stage::icon_map = nullptr;
float Stage::interface_scale = 1.0f;

void Stage::StyleSheetPolish(bwWidget& widget)
{
  StyleSheet& stylesheet = *Stage::style_sheet;

  for (auto& property : widget.style_properties) {
    stylesheet.resolveValue(widget.getIdentifier(), widget.state, *property);
  }
}

Stage::Stage(const unsigned int width, const unsigned int height)
    : screen_graph(bwPtr_new<bwScreenGraph::LayoutNode>()), mask_width(width), mask_height(height)
{
  initFonts();
  initIcons();

  // After font-init!
  bwPainter::paint_engine = bwPtr_new<GawainPaintEngine>(*font, *icon_map);
  bwStyleCSS::polish_cb = Stage::StyleSheetPolish;

  bwStyleManager& style_manager = bwStyleManager::getStyleManager();
  style_manager.registerDefaultStyleTypes();
  activateStyleID(bwStyle::TypeID::CLASSIC);

  setFontTightPositioning(true);

  auto layout = bwPtr_new<RootLayout>(height, width);
  layout->padding = 7;
  layout->item_margin = 5;
  bwScreenGraph::Builder::setLayout(screen_graph.Root(), std::move(layout));
}

void Stage::initFonts()
{
  // Initialize freetype
  Font::initFontReading();

  // Initialize default font
  font = bwPtr<Font>(Font::loadFont("bfont.ttf", RESOURCES_PATH_STR));
  font->setSize(11.0f * interface_scale);
}

void Stage::initIcons()
{
  IconMapReader reader;
  File png_file(RESOURCES_PATH_STR + std::string("/blender_icons16.png"), std::ios::binary);

  icon_map = reader.readIconMapFromPNGFile(png_file);
}

void Stage::activateStyleID(bwStyle::TypeID type_id)
{
  style = bwPtr<bwStyle>(bwStyleManager::createStyleFromTypeID(type_id));
  style->dpi_fac = interface_scale;
}

namespace bWidgetsDemo {

class ScrollbarApplyValueFunctor : public bwFunctorInterface {
  Stage& stage;
  const bwScrollBar& scrollbar;

 public:
  ScrollbarApplyValueFunctor(Stage& stage, const bwScrollBar& scrollbar)
      : stage(stage), scrollbar(scrollbar)
  {
  }
  inline void operator()() override
  {
    stage.setScrollValue(scrollbar.scroll_offset);
  }
};

}  // namespace bWidgetsDemo

void Stage::drawScrollbars()
{
  using namespace bwScreenGraph;

  if (shouldHaveScrollbars()) {
    const unsigned int padding = 4u * (unsigned int)interface_scale;
    bwScrollBar* scrollbar = static_cast<bwScrollBar*>(scrollbar_node.Widget());

    if (!scrollbar) {
      Builder::setWidget(scrollbar_node, bwPtr_new<bwScrollBar>(getScrollbarWidth(), mask_height));
      scrollbar = static_cast<bwScrollBar*>(scrollbar_node.Widget());
      scrollbar->apply_functor = bwPtr_new<ScrollbarApplyValueFunctor>(*this, *scrollbar);
    }

    scrollbar->rectangle = bwRectanglePixel(
        getContentWidth(), mask_width - padding, padding, mask_height - padding);
    scrollbar->ratio = mask_height / (float)getContentHeight();
    scrollbar->scroll_offset = vert_scroll;
    scrollbar->draw(*style);
  }
  else if (scrollbar_node.Widget()) {
    Builder::setWidget(scrollbar_node, nullptr);
  }
}

static void drawScreenGraph(bwScreenGraph::ScreenGraph& screen_graph, bwStyle& style)
{
  const bwScreenGraph::Node* skip_until_parent = nullptr;

  for (auto& iter_node : screen_graph) {
    bwWidget* widget = iter_node.Widget();

    if (skip_until_parent && (skip_until_parent == iter_node.Parent())) {
      skip_until_parent = nullptr;
    }

    if (skip_until_parent || !widget || widget->hidden || widget->rectangle.isEmpty()) {
      continue;
    }

    bwPanel* panel = widget_cast<bwPanel*>(widget);
    if (panel && (panel->panel_state == bwPanel::State::CLOSED)) {
      skip_until_parent = iter_node.Parent();
    }

    widget->draw(style);
  }
}

void Stage::draw()
{
  bwRectanglePixel rect{0, (int)mask_width, 0, (int)mask_height};
  bwStyleProperties properties;
  bwColor clear_color{114u};

  if (style->type_id == bwStyle::TypeID::CLASSIC_CSS) {
    setStyleSheet(std::string(RESOURCES_PATH_STR) + "/" + "classic_style.css");
  }
  else if (style->type_id == bwStyle::TypeID::FLAT_LIGHT) {
    setStyleSheet(std::string(RESOURCES_PATH_STR) + "/" + "flat_light.css");
  }
  else if (style->type_id == bwStyle::TypeID::FLAT_DARK) {
    setStyleSheet(std::string(RESOURCES_PATH_STR) + "/" + "flat_dark.css");
  }
  else {
    style_sheet = nullptr;
  }

  bwStyleProperty& property = properties.addColor("background-color", clear_color);
  if (style_sheet) {
    style_sheet->resolveValue("Stage", bwWidget::State::NORMAL, property);
  }

  bwPainter::paint_engine->setupViewport(rect, clear_color);

  updateContentBounds();

  resolveScreenGraphNodeLayout(screen_graph.Root(), vert_scroll, interface_scale);
  drawScreenGraph(screen_graph, *style);
  drawScrollbars();
}

void Stage::setInterfaceScale(const float value)
{
  if (value != interface_scale) {
    interface_scale = value;
    font->setSize(11.0f * value);
    style->dpi_fac = value;
  }
}

void Stage::setFontAntiAliasingMode(const Font::AntiAliasingMode aa_mode)
{
  font->setFontAntiAliasingMode(aa_mode);
}

void Stage::setFontTightPositioning(const bool value)
{
  font->setTightPositioning(value);
}

void Stage::setFontHinting(const bool value)
{
  font->setHinting(value);
}

void Stage::setFontSubPixelPositioning(const bool value)
{
  font->setSubPixelPositioning(value);
}

RootLayout& Stage::Layout() const
{
  return static_cast<RootLayout&>(*screen_graph.Root().Layout());
}

void Stage::setStyleSheet(const std::string& filepath)
{
  if (!style_sheet || (style_sheet->getFilepath() != filepath)) {
    style_sheet = bwPtr_new<StyleSheet>(filepath);
  }
  else {
    /* TODO skip if file didn't change. */
    style_sheet->reload();
  }
}

void Stage::updateContentBounds()
{
  RootLayout& layout = Layout();

  validizeScrollValue();
  layout.setMaxSize(getContentWidth());
  layout.setYmax(mask_height);
}

void Stage::validizeScrollValue()
{
  if (shouldHaveScrollbars()) {
    bwRange<int>::clampValue(vert_scroll, mask_height - getContentHeight(), 0);
  }
}

void Stage::setScrollValue(int value)
{
  vert_scroll = value;
  validizeScrollValue();
}

void Stage::handleMouseMovementEvent(const MouseEvent& event)
{
  using namespace bwScreenGraph;

  const bwPoint& mouse_location = event.getMouseLocation();

  // TODO Multiple hovered items need to be possible (e.g. button + surrounding panel).

  screen_graph.event_dispatcher.dispatchMouseMovement(bwEvent(mouse_location));

  if (!screen_graph.context.hovered && scrollbar_node.Widget() &&
      scrollbar_node.Widget()->isCoordinateInside(mouse_location)) {
    screen_graph.event_dispatcher.changeContextHovered(&scrollbar_node);
  }
}

void Stage::handleMouseButtonEvent(const MouseEvent& event)
{
  bwMouseButtonEvent bw_event(event.getButton(), event.getMouseLocation());
  bwEventDispatcher& dispatcher = screen_graph.event_dispatcher;

  switch (event.getType()) {
    case MouseEvent::MOUSE_EVENT_PRESS:
      dispatcher.dispatchMouseButtonPress(bw_event);
      break;
    case MouseEvent::MOUSE_EVENT_RELEASE:
      dispatcher.dispatchMouseButtonRelease(bw_event);
      break;
    default:
      break;
  }
}

void Stage::handleMouseScrollEvent(const MouseEvent& event)
{
  if (shouldHaveScrollbars()) {
    char direction_fac = 0;

    if (event.getType() == MouseEvent::MOUSE_EVENT_SCROLL_DOWN) {
      direction_fac = -1;
    }
    else if (event.getType() == MouseEvent::MOUSE_EVENT_SCROLL_UP) {
      direction_fac = 1;
    }

    setScrollValue(vert_scroll + (direction_fac * 40));
  }
}

void Stage::handleWindowResizeEvent(const Window& win)
{
  mask_width = win.getWidth();
  mask_height = win.getHeight();
}

unsigned int Stage::getScrollbarWidth() const
{
  return std::round(17 * interface_scale);
}

unsigned int Stage::getContentWidth() const
{
  if (shouldHaveScrollbars()) {
    return (getScrollbarWidth() > mask_width) ? 0u : (mask_width - getScrollbarWidth());
  }

  return mask_width;
}

unsigned int Stage::getContentHeight() const
{
  return Layout().getHeight() +
         (2 * Layout().padding);  // TODO Padding should actually be added to layout width/height
}

bool Stage::shouldHaveScrollbars() const
{
  return (mask_height < getContentHeight()) || (vert_scroll != 0);
}
