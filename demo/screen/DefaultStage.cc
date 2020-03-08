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

#include "IconMap.h"
#include "Layout.h"

#include "bwCheckbox.h"
#include "bwLabel.h"
#include "bwNumberSlider.h"
#include "bwPanel.h"
#include "bwPushButton.h"
#include "bwRadioButton.h"
#include "bwStyleManager.h"
#include "screen_graph/Builder.h"
#include "screen_graph/Iterators.h"

#include "DefaultStage.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo {

#define BUTTON_HEIGHT 20
#define PANEL_HEADER_HEIGHT 24

class DefaultStageRNAFunctor : public bwFunctorInterface {
 public:
  DefaultStageRNAFunctor(RNAProperties<DefaultStage>& props,
                         DefaultStage& stage,
                         const std::string& prop_name,
                         const bwWidget& widget)
      : m_props(props), m_stage(stage), m_prop_name(prop_name), m_widget(widget)
  {
  }

  DefaultStageRNAFunctor(RNAProperties<DefaultStage>& props,
                         DefaultStage& stage,
                         const std::string& prop_name,
                         const bwWidget& widget,
                         int enum_value)
      : DefaultStageRNAFunctor(props, stage, prop_name, widget)
  {
    m_enum_value = enum_value;
  }

  void operator()() override
  {
    if (widget_cast<const bwCheckbox*>(&m_widget)) {
      m_props.set(m_prop_name, m_stage, m_widget.state == bwWidget::State::SUNKEN);
    }
    else if (auto* slider = widget_cast<const bwNumberSlider*>(&m_widget)) {
      m_props.set(m_prop_name, m_stage, slider->getValue());
    }
    else if (widget_cast<const bwRadioButton*>(&m_widget)) {
      m_props.set(m_prop_name, m_stage, m_enum_value.value());
    }
  }

  const std::string& getPropName() const
  {
    return m_prop_name;
  }

  bwOptional<int> getEnumValue() const
  {
    return m_enum_value;
  }

 private:
  RNAProperties<DefaultStage>& m_props;
  DefaultStage& m_stage;
  std::string m_prop_name;
  const bwWidget& m_widget;
  bwOptional<int> m_enum_value;
};

// --------------------------------------------------------------------

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height)
    : Stage(mask_width, mask_height)
{
  using namespace bwScreenGraph;
  Builder builder(screen_graph);
  ContainerNode* panel;

  registerProperties(properties);

  addStyleSelector(screen_graph.Root());

  auto& slider = builder.addWidget<bwNumberSlider>(0, BUTTON_HEIGHT);
  slider.apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
      properties, *this, "interface_scale", slider);
  slider.setText("Interface Scale: ");
  slider.setMinMax(0.5f, 2.0f);
  slider.setValue(1.0f);

  builder.addWidget<bwLabel>("Font Rendering:", 0, BUTTON_HEIGHT);

  builder.addLayout<RowLayout>(true);
  auto* checkbox = &builder.addWidget<bwCheckbox>("Tight Positioning", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
      properties, *this, "font_use_tight_positioning", *checkbox);
  checkbox->state = bwWidget::State::SUNKEN;
  checkbox = &builder.addWidget<bwCheckbox>("Hinting", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
      properties, *this, "font_use_hinting", *checkbox);

  builder.setActiveLayout(screen_graph.Root());
  builder.addLayout<RowLayout>(false);
  checkbox = &builder.addWidget<bwCheckbox>("Subpixel Rendering", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
      properties, *this, "font_use_subpixels", *checkbox);
  checkbox = &builder.addWidget<bwCheckbox>("Subpixel Positioning", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
      properties, *this, "font_use_subpixel_positioning", *checkbox);
  checkbox->hidden = true;

  builder.setActiveLayout(screen_graph.Root());
  panel = &builder.addContainer<bwPanel>(
      bwPtr_new<PanelLayout>(), "Some Testing Widgets", PANEL_HEADER_HEIGHT);
  builder.addLayout<ColumnLayout>(true);
  builder.addWidget<bwPushButton>("Translate", 0, BUTTON_HEIGHT);
  builder.addWidget<bwPushButton>("Rotate", 0, BUTTON_HEIGHT);
  builder.addWidget<bwPushButton>("Scale", 0, BUTTON_HEIGHT);

  builder.setActiveLayout(*panel);
  auto& push_but = builder.addWidget<bwPushButton>("Mirror", 0, BUTTON_HEIGHT);
  push_but.setIcon(icon_map->getIcon(ICON_MOD_MIRROR));

  builder.setActiveLayout(screen_graph.Root());
  panel = &builder.addContainer<bwPanel>(
      bwPtr_new<PanelLayout>(), "More Testing...", PANEL_HEADER_HEIGHT);
  builder.addLayout<RowLayout>(true);
  builder.addWidget<bwCheckbox>("Make Awesome", 0, BUTTON_HEIGHT);
  builder.addWidget<bwCheckbox>("Wireframes", 0, BUTTON_HEIGHT);

  builder.setActiveLayout(*panel);
  auto& text_box = builder.addWidget<bwTextBox>(0, BUTTON_HEIGHT);
  text_box.setText("Some Text...");

  builder.addLayout<RowLayout>(false);
  auto* label = &builder.addWidget<bwLabel>("Pose Icon", 0, BUTTON_HEIGHT);
  label->setIcon(icon_map->getIcon(ICON_POSE_HLT));
  label = &builder.addWidget<bwLabel>("Normalized FCurve Icon", 0, BUTTON_HEIGHT);
  label->setIcon(icon_map->getIcon(ICON_NORMALIZE_FCURVES));
  label = &builder.addWidget<bwLabel>("Chroma Scope Icon", 0, BUTTON_HEIGHT);
  label->setIcon(icon_map->getIcon(ICON_SEQ_CHROMA_SCOPE));
}

bool isUseCSSVersionToggleHidden(const bwStyle& style)
{
  return (style.type_id != bwStyle::TypeID::CLASSIC) &&
         (style.type_id != bwStyle::TypeID::CLASSIC_CSS);
}

void DefaultStage::addStyleSelector(bwScreenGraph::LayoutNode& parent_node)
{
  using namespace bwScreenGraph;
  Builder builder(parent_node);

  builder.addLayout<RowLayout>(true);

  auto& label = builder.addWidget<bwLabel>("Style:", 0, BUTTON_HEIGHT);
  label.setIcon(icon_map->getIcon(ICON_BLENDER));

  for (const bwStyle::StyleType& type : bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
    if (type.type_id == bwStyle::TypeID::CLASSIC_CSS) {
      // We'll add a button for this later.
      continue;
    }
    auto& style_button = builder.addWidget<bwRadioButton>(type.name, 0, BUTTON_HEIGHT);

    style_button.apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
        properties, *this, "style_type", style_button, int(type.type_id));

    if (type.type_id == style->type_id) {
      style_button.state = bwAbstractButton::State::SUNKEN;
    }
  }

  builder.setActiveLayout(parent_node);
  auto& checkbox = builder.addWidget<bwCheckbox>("Use CSS Version", 0, BUTTON_HEIGHT);
  checkbox.hidden = isUseCSSVersionToggleHidden(*style);
  checkbox.apply_functor = bwPtr_new<DefaultStageRNAFunctor>(
      properties, *this, "style_use_css_version", checkbox);
}

void DefaultStage::useStyleCSSVersionSet(const bool use_css_version)
{
  bwStyle::TypeID active_type_id = style->type_id;

  assert(active_type_id == bwStyle::TypeID::CLASSIC ||
         active_type_id == bwStyle::TypeID::CLASSIC_CSS);
  if (use_css_version) {
    if (active_type_id != bwStyle::TypeID::CLASSIC_CSS) {
      activateStyleID(bwStyle::TypeID::CLASSIC_CSS);
    }
  }
  else {
    if (active_type_id != bwStyle::TypeID::CLASSIC) {
      activateStyleID(bwStyle::TypeID::CLASSIC);
    }
  }
}

bool DefaultStage::updateStyleButton(bwWidget& widget_iter)
{
  bwStyle::TypeID active_type_id = DefaultStage::style->type_id;

  if (auto* radio_iter = widget_cast<bwRadioButton*>(&widget_iter)) {
    if (radio_iter->apply_functor) {
      auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(radio_iter->apply_functor.get());

      if (rna_functor && rna_functor->getPropName() == "style_type") {
        auto prop = rna_functor->getEnumValue();
        if (prop && bwStyle::TypeID(prop.value()) == active_type_id) {
          radio_iter->state = bwWidget::State::SUNKEN;
        }
        else {
          radio_iter->state = bwWidget::State::NORMAL;
        }
      }
    }
  }
  else if (auto* checkbox_iter = widget_cast<bwCheckbox*>(&widget_iter)) {
    if (checkbox_iter->apply_functor) {
      auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(
          checkbox_iter->apply_functor.get());
      if (rna_functor && rna_functor->getPropName() == "style_use_css_version") {
        if (active_type_id == bwStyle::TypeID::CLASSIC ||
            active_type_id == bwStyle::TypeID::CLASSIC_CSS) {
          useStyleCSSVersionSet(checkbox_iter->state == bwWidget::State::SUNKEN);
        }
      }
    }
  }

  return true;
}

void DefaultStage::updateStyleButtons()
{
  // Deactivate style radio buttons that are not active. In future this should be handled within
  // bWidgets somehow (groups of radio buttons and a value getter maybe?).
  for (bwScreenGraph::Node& node : screen_graph) {
    if (!node.isVisible()) {
      continue;
    }
    bwWidget* widget = node.Widget();
    updateStyleButton(*widget);
  }
}

void DefaultStage::activateStyleID(bwStyle::TypeID type_id)
{
  Stage::activateStyleID(type_id);
  for (auto& iter_node : screen_graph) {
    bwWidget* widget = iter_node.Widget();
    if (!widget) {
      continue;
    }

    const auto* checkbox = widget_cast<bwCheckbox*>(widget);
    if (checkbox && checkbox->apply_functor) {
      const auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(
          checkbox->apply_functor.get());
      if (rna_functor && (rna_functor->getPropName() == "style_use_css_version")) {
        widget->hidden = isUseCSSVersionToggleHidden(*Stage::style);
      }
    }
  }
}

void DefaultStage::updateFontAAMode(bool value)
{
  for (bwScreenGraph::Node& node : screen_graph.Root()) {
    bwWidget* widget = node.Widget();
    if (!widget) {
      continue;
    }
    if (auto* iter_checkbox = widget_cast<bwCheckbox*>(widget)) {
      if (iter_checkbox->apply_functor) {
        const auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(
            iter_checkbox->apply_functor.get());
        if (rna_functor && (rna_functor->getPropName() == "font_use_subpixel_positioning")) {
          iter_checkbox->hidden = !value;
        }
      }
    }
  }
}

void DefaultStage::registerProperties(RNAProperties<DefaultStage>& properties)
{
  properties.defProperty<bwStyle::TypeID>(
      "style_type",
      [](DefaultStage& stage) { return stage.style->type_id; },
      [](DefaultStage& stage, bwStyle::TypeID value) {
        stage.activateStyleID(value);
        stage.updateStyleButtons();
      });
  properties.defProperty<bool>(
      "style_use_css_version",
      [](DefaultStage&) { return style->type_id == bwStyle::TypeID::CLASSIC_CSS; },
      [](DefaultStage& stage, bool value) { stage.useStyleCSSVersionSet(value); });

  properties.defProperty<bool>(
      "font_use_tight_positioning",
      [](DefaultStage&) { return true; }, /* TODO */
      [](DefaultStage&, bool value) { Stage::setFontTightPositioning(value); });
  properties.defProperty<bool>(
      "font_use_hinting",
      [](DefaultStage&) { return true; }, /* TODO */
      [](DefaultStage&, bool value) { Stage::setFontHinting(value); });
  properties.defProperty<bool>(
      "font_use_subpixels",
      [](DefaultStage&) { return true; }, /* TODO */
      [](DefaultStage& stage, bool value) {
        Stage::setFontAntiAliasingMode(value ? Font::SUBPIXEL_LCD_RGB_COVERAGE :
                                               Font::NORMAL_COVERAGE);
        stage.updateFontAAMode(value);
      });
  properties.defProperty<bool>(
      "font_use_subpixel_positioning",
      [](DefaultStage&) { return true; }, /* TODO */
      [](DefaultStage& stage, bool value) { stage.setFontSubPixelPositioning(value); });

  properties.defProperty<float>(
      "interface_scale",
      [](DefaultStage& stage) { return stage.interface_scale; }, /* TODO */
      [](DefaultStage& stage, float value) { stage.setInterfaceScale(value); });
}

}  // namespace bWidgetsDemo
