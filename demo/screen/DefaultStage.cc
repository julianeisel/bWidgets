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

#include "blender_icon_defines.h"

#include "DefaultStageRNAFunctor.h"
#include "DummyFunctors.hh"
#include "IconMap.h"
#include "Layout.h"
#include "RNABinding.h"
#include "RNAScreenGraphBuilder.h"

#include "builtin_widgets.h"
#include "bwStyleManager.h"
#include "screen_graph/Builder.h"
#include "screen_graph/Iterators.h"

#include "DefaultStage.h"
#include "DefaultStageRNAFunctor.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo {

#define PANEL_HEADER_HEIGHT 24

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height)
    : Stage(mask_width, mask_height)
{
  registerProperties(properties);
}

void DefaultStage::constructUI(bwScreenGraph::LayoutNode& parent)
{
  using namespace bwScreenGraph;
  /* Convenience */
  using RNABuilder = RNAScreenGraphBuilder<DefaultStage, DefaultStageRNAFunctor>;

  RNABuilder builder(parent, *this, properties);

  addStyleSelector(parent);

  builder.addRNAWidget<bwNumberSlider>("interface_scale")
      .setMinMax(0.5f, 2.0f)
      .setText("Interface Scale:");

  builder.addWidget<bwLabel>("Font Rendering:");

  builder.buildLayout<RowLayout, RNABuilder>([this](RNABuilder& builder) {
    builder.addRNAWidget<bwCheckbox>("font_use_tight_positioning").setLabel("Tight Positioning");
    builder.addRNAWidget<bwCheckbox>("font_use_hinting").setLabel("Hinting");
  });

  builder.buildLayout<RowLayout, RNABuilder>([this](RNABuilder& builder) {
    builder.addRNAWidget<bwCheckbox>("font_use_subpixels").setLabel("Subpixel Rendering");

    if (*properties.get<bool>("font_use_subpixels", *this)) {
      builder.addRNAWidget<bwCheckbox>("font_use_subpixel_positioning")
          .setLabel("Subpixel Positioning");
    }
  });

  builder.buildContainer<bwPanel, RNABuilder>(
      [](RNABuilder& builder) {
        builder.buildLayout<ColumnLayout>(
            [](Builder& builder) {
              builder.addWidget<bwPushButton>("Translate");
              builder.addWidget<bwPushButton>("Rotate");
              builder.addWidget<bwPushButton>("Scale");
            },
            true);

        builder.addWidget<bwPushButton>("Mirror").setIcon(icon_map->getIcon(ICON_MOD_MIRROR));
      },
      std::make_unique<PanelLayout>(),
      "Some Testing Widgets",
      PANEL_HEADER_HEIGHT);

  builder.buildContainer<bwPanel>(
      [](Builder& builder) {
        builder.buildLayout<RowLayout>([](Builder& builder) {
          static bool use_make_awesome = false;
          builder.addWidget<bwCheckbox>(Binding(use_make_awesome), "Make Awesome");
          static bool use_wireframes = false;
          builder.addWidget<bwCheckbox>(Binding(use_wireframes), "Wireframes");
        });

        builder.addWidget<bwTextBox>().setText("Some Text...");

        builder.buildLayout<RowLayout>([](Builder& builder) {
          builder.addWidget<bwLabel>()
              .setLabel("Pose Icon")
              .setIcon(icon_map->getIcon(ICON_POSE_HLT));
          builder.addWidget<bwLabel>()
              .setLabel("Normalized FCurve Icon")
              .setIcon(icon_map->getIcon(ICON_NORMALIZE_FCURVES));
          builder.addWidget<bwLabel>()
              .setLabel("Chroma Scope Icon")
              .setIcon(icon_map->getIcon(ICON_SEQ_CHROMA_SCOPE));
        });
      },
      std::make_unique<PanelLayout>(),
      "More Testing...",
      PANEL_HEADER_HEIGHT);
}

auto isUseCSSVersionToggleHidden(const bwStyle& style) -> bool
{
  return (style.type_id != bwStyle::TypeID::CLASSIC) &&
         (style.type_id != bwStyle::TypeID::CLASSIC_CSS);
}

void DefaultStage::addStyleSelector(bwScreenGraph::LayoutNode& parent_node)
{
  /* Convenience */
  using namespace bwScreenGraph;
  using RNABuilder = RNAScreenGraphBuilder<DefaultStage, DefaultStageRNAFunctor>;
  RNABuilder builder(parent_node, *this, properties);

  builder.buildLayout<RowLayout, RNABuilder>(
      [](RNABuilder& builder) {
        builder.addWidget<bwLabel>("Style:").setIcon(icon_map->getIcon(ICON_BLENDER));

        for (const bwStyle::StyleType& type :
             bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
          if (type.type_id == bwStyle::TypeID::CLASSIC_CSS) {
            // A checkbox button is added for this variation later.
            continue;
          }

          auto& style_button = builder.addRNAWidget<bwRadioButton>(
              int(type.type_id), "style_type", type.name);

          if (type.type_id == style->type_id) {
            style_button.setBaseState(bwWidgetState::SUNKEN);
          }
          else if ((type.type_id == bwStyle::TypeID::CLASSIC) &&
                   style->type_id == bwStyle::TypeID::CLASSIC_CSS) {
            style_button.setBaseState(bwWidgetState::SUNKEN);
          }
        }
      },
      true);

  if (!isUseCSSVersionToggleHidden(*style)) {
    builder.addRNAWidget<bwCheckbox>("style_use_css_version", "Use CSS Version");
  }
}

void DefaultStage::useStyleCSSVersionSet(const bool use_css_version)
{
  bwStyle::TypeID active_type_id = style->type_id;

  assert(active_type_id == bwStyle::TypeID::CLASSIC ||
         active_type_id == bwStyle::TypeID::CLASSIC_CSS);
  activateStyleID(use_css_version ? bwStyle::TypeID::CLASSIC_CSS : bwStyle::TypeID::CLASSIC);
}

void DefaultStage::registerProperties(RNAProperties<DefaultStage>& properties)
{
  properties.defProperty<bwStyle::TypeID>(
      "style_type",
      [](const DefaultStage& stage) { return stage.style->type_id; },
      [](DefaultStage& stage, bwStyle::TypeID value) { stage.activateStyleID(value); });
  properties.defProperty<bool>(
      "style_use_css_version",
      [](const DefaultStage&) { return style->type_id == bwStyle::TypeID::CLASSIC_CSS; },
      [](DefaultStage& stage, bool value) { stage.useStyleCSSVersionSet(value); });

  properties.defProperty<bool>(
      "font_use_tight_positioning",
      [](const DefaultStage&) { return Stage::getFontTightPositioning(); },
      [](DefaultStage&, bool value) { Stage::setFontTightPositioning(value); });
  properties.defProperty<bool>(
      "font_use_hinting",
      [](const DefaultStage&) { return Stage::getFontHinting(); },
      [](DefaultStage&, bool value) { Stage::setFontHinting(value); });
  properties.defProperty<bool>(
      "font_use_subpixels",
      [](const DefaultStage&) {
        return Stage::getFontAntiAliasingMode() == Font::SUBPIXEL_LCD_RGB_COVERAGE;
      },
      [](DefaultStage&, bool value) {
        Stage::setFontAntiAliasingMode(value ? Font::SUBPIXEL_LCD_RGB_COVERAGE :
                                               Font::NORMAL_COVERAGE);
      });
  properties.defProperty<bool>(
      "font_use_subpixel_positioning",
      [](const DefaultStage&) { return Stage::getFontSubPixelPositioning(); },
      [](DefaultStage&, bool value) { Stage::setFontSubPixelPositioning(value); });

  properties.defProperty<float>(
      "interface_scale",
      [](const DefaultStage&) { return Stage::interface_scale; },
      [](DefaultStage&, float value) { Stage::setInterfaceScale(value); });
}

}  // namespace bWidgetsDemo
