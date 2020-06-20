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
#include <cmath>
#include <iostream>

// bWidgets lib
#include "bwPainter.h"
#include "bwRange.h"
#include "bwRectangle.h"
#include "bwScrollView.h"
#include "bwStyleCSS.h"
#include "bwStyleManager.h"
#include "screen_graph/Builder.h"
#include "screen_graph/Drawer.h"
#include "screen_graph/Iterators.h"

#include "Event.h"
#include "File.h"
#include "GPUShader.h"
#include "GawainPaintEngine.h"
#include "IconMap.h"
#include "Layout.h"
#include "StyleSheet.h"
#include "Window.h"

#include "Stage.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo {

std::unique_ptr<bwStyle> Stage::style = nullptr;
std::unique_ptr<StyleSheet> Stage::style_sheet = nullptr;
std::unique_ptr<Font> Stage::font = nullptr;
std::unique_ptr<IconMap> Stage::icon_map = nullptr;
float Stage::interface_scale = 1.0f;

bwScreenGraph::ScreenGraph createScreenGraph(const unsigned int width, const unsigned int height)
{
  auto container = std::make_unique<bwScreenGraph::ContainerNode>();
  auto layout = std::make_unique<ScrollViewLayout>();
  auto scroll_view = std::make_unique<bwScrollView>(*container, width, height);

  layout->padding = 7;
  layout->item_margin = 5;
  bwScreenGraph::Builder::setLayout(*container, std::move(layout));
  bwScreenGraph::Builder::setWidget(*container, std::move(scroll_view));

  return bwScreenGraph::ScreenGraph(std::move(container));
}

Stage::Stage(const unsigned int width, const unsigned int height)
    : screen_graph(createScreenGraph(width, height)), mask_width(width), mask_height(height)
{
  initFonts();
  initIcons();

  // After font-init!
  bwPainter::paint_engine = std::make_unique<GawainPaintEngine>(*font, *icon_map);
  bwStyleCSS::polish_cb = Stage::StyleSheetPolish;

  bwStyleManager& style_manager = bwStyleManager::getStyleManager();
  style_manager.registerDefaultStyleTypes();
  activateStyleID(bwStyle::TypeID::CLASSIC);

  setFontTightPositioning(true);
}

Stage::~Stage()
{
  GPUShader::clearCache();
}

void Stage::initFonts()
{
  // Initialize freetype
  Font::initFontReading();

  // Initialize default font
  font = std::unique_ptr<Font>(Font::loadFont("bfont.ttf", RESOURCES_PATH_STR));
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
  style = std::unique_ptr<bwStyle>(bwStyleManager::createStyleFromTypeID(type_id));
  style->dpi_fac = interface_scale;
}

void Stage::draw()
{
  bwRectanglePixel stage_rect{0, int(mask_width) - 1, 0, int(mask_height - 1)};
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

  bwPainter::paint_engine->setupViewport(stage_rect, clear_color);

  resolveScreenGraphNodeLayout(screen_graph.Root(), stage_rect, interface_scale);
  bwScreenGraph::Drawer::draw(screen_graph, *style);
}

void Stage::StyleSheetPolish(bwWidget& widget)
{
  StyleSheet& stylesheet = *Stage::style_sheet;

  for (auto& property : widget.style_properties) {
    stylesheet.resolveValue(widget.getIdentifier(), widget.state, *property);
  }
}

void Stage::setContentScale(const float scale_x, const float scale_y)
{
  auto& gwn_engine = dynamic_cast<GawainPaintEngine&>(*bwPainter::paint_engine);
  gwn_engine.m_scale_x = scale_x;
  gwn_engine.m_scale_y = scale_y;
  setFontSize(11.0f);
}

void Stage::setInterfaceScale(const float value)
{
  if (value != interface_scale) {
    interface_scale = value;
    style->dpi_fac = value;
    setFontSize(11.0f);
  }
}

void Stage::setFontSize(const float size)
{
  auto& gwn_engine = dynamic_cast<GawainPaintEngine&>(*bwPainter::paint_engine);
  font->setSize(size * interface_scale * gwn_engine.m_scale_x);
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

void Stage::setStyleSheet(const std::string& filepath)
{
  if (!style_sheet || (style_sheet->getFilepath() != filepath)) {
    style_sheet = std::make_unique<StyleSheet>(filepath);
  }
  else {
    /* TODO skip if file didn't change. */
    style_sheet->reload();
  }
}

void Stage::handleMouseMovementEvent(const MouseEvent& event)
{
  using namespace bwScreenGraph;

  const bwPoint& mouse_location = event.getMouseLocation();

  // TODO Multiple hovered items need to be possible (e.g. button + surrounding panel).

  screen_graph.event_dispatcher.dispatchMouseMovement(bwEvent(mouse_location));
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

void Stage::handleMouseScrollEvent(const MouseEvent& event, bwMouseWheelEvent::Direction dir)
{
  bwMouseWheelEvent bw_event(dir, event.getMouseLocation());
  bwEventDispatcher& dispatcher = screen_graph.event_dispatcher;

  dispatcher.dispatchMouseWheelScroll(bw_event);
}

void Stage::handleWindowResizeEvent(const Window& win)
{
  mask_width = win.getWidth();
  mask_height = win.getHeight();
}

}  // namespace bWidgetsDemo
