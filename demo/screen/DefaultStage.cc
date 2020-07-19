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

#define PANEL_HEADER_HEIGHT 24

class DefaultStageRNAFunctor : public bwFunctorInterface {
 public:
  DefaultStageRNAFunctor(RNAProperties<DefaultStage>& props,
                         DefaultStage& stage,
                         const std::string& prop_name,
                         const bwWidget& widget)
      : m_props(props), m_stage(stage), m_prop_name(prop_name), m_widget(widget)
  {
    /* Sanity check. */
    assert(props.find(prop_name));
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
    if (widget_cast<bwCheckbox>(m_widget)) {
      m_props.set(m_prop_name, m_stage, m_widget.getState() == bwWidget::State::SUNKEN);
    }
    else if (auto* slider = widget_cast<bwNumberSlider>(m_widget)) {
      m_props.set(m_prop_name, m_stage, slider->getValue());
    }
    else if (widget_cast<bwRadioButton>(m_widget)) {
      m_props.set(m_prop_name, m_stage, m_enum_value.value());
    }
  }

  const std::string& getPropName() const
  {
    return m_prop_name;
  }

  std::optional<int> getEnumValue() const
  {
    return m_enum_value;
  }

 private:
  RNAProperties<DefaultStage>& m_props;
  DefaultStage& m_stage;
  std::string m_prop_name;
  const bwWidget& m_widget;
  std::optional<int> m_enum_value;
};

template<typename _Obj> class RNAScreenGraphBuilder : public bwScreenGraph::Builder {
 public:
  RNAScreenGraphBuilder(bwScreenGraph::LayoutNode& node,
                        _Obj& obj,
                        RNAProperties<_Obj>& properties)
      : bwScreenGraph::Builder(node), m_obj(obj), m_props(properties)
  {
  }
  RNAScreenGraphBuilder(bwScreenGraph::ScreenGraph& screen_graph,
                        _Obj& obj,
                        RNAProperties<_Obj>& properties)
      : RNAScreenGraphBuilder(screen_graph.Root(), obj, properties)
  {
  }

  template<typename _WidgetType, typename... _Args>
  _WidgetType& addRNAWidget(const std::string& propname, _Args&&... __args)
  {
    _WidgetType& widget = Builder::addWidget<_WidgetType>(std::forward<_Args>(__args)...);
    static_assert(!std::is_same<_WidgetType, bwRadioButton>::value,
                  "RNAScreenGraphBuilder: For bwRadioButton, addRNAWidget overload with enum "
                  "value should be called.");
    widget.apply_functor = std::make_unique<DefaultStageRNAFunctor>(
        m_props, m_obj, propname, widget);
    return widget;
  }

  template<typename _WidgetType, typename... _Args>
  _WidgetType& addRNAWidget(int enum_value, const std::string& propname, _Args&&... __args)
  {
    _WidgetType& widget = Builder::addWidget<_WidgetType>(std::forward<_Args>(__args)...);
    widget.apply_functor = std::make_unique<DefaultStageRNAFunctor>(
        m_props, m_obj, propname, widget, enum_value);
    return widget;
  }

 private:
  _Obj& m_obj;
  RNAProperties<_Obj>& m_props;
};

// --------------------------------------------------------------------

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height)
    : Stage(mask_width, mask_height)
{
  using namespace bwScreenGraph;

  RNAScreenGraphBuilder<DefaultStage> builder(screen_graph, *this, properties);
  ContainerNode* panel;

  registerProperties(properties);

  addStyleSelector(screen_graph.Root());

  builder.addRNAWidget<bwNumberSlider>("interface_scale")
      .setMinMax(0.5f, 2.0f)
      .setValue(1.0f)
      .setText("Interface Scale: ");

  builder.addWidget<bwLabel>("Font Rendering:");

  builder.addLayout<RowLayout>(true);
  builder.addRNAWidget<bwCheckbox>("font_use_tight_positioning", "Tight Positioning")
      .setState(bwWidget::State::SUNKEN);
  builder.addRNAWidget<bwCheckbox>("font_use_hinting", "Hinting");

  builder.setActiveLayout(screen_graph.Root());
  builder.addLayout<RowLayout>(false);
  builder.addRNAWidget<bwCheckbox>("font_use_subpixels", "Subpixel Rendering");
  builder.addRNAWidget<bwCheckbox>("font_use_subpixel_positioning", "Subpixel Positioning").hide();

  builder.setActiveLayout(screen_graph.Root());
  panel = &builder.addContainer<bwPanel>(
      std::make_unique<PanelLayout>(), "Some Testing Widgets", PANEL_HEADER_HEIGHT);
  builder.addLayout<ColumnLayout>(true);
  builder.addWidget<bwPushButton>("Translate");
  builder.addWidget<bwPushButton>("Rotate");
  builder.addWidget<bwPushButton>("Scale");

  builder.setActiveLayout(*panel);
  builder.addWidget<bwPushButton>("Mirror").setIcon(icon_map->getIcon(ICON_MOD_MIRROR));

  builder.setActiveLayout(screen_graph.Root());
  panel = &builder.addContainer<bwPanel>(
      std::make_unique<PanelLayout>(), "More Testing...", PANEL_HEADER_HEIGHT);
  builder.addLayout<RowLayout>(true);
  builder.addWidget<bwCheckbox>("Make Awesome");
  builder.addWidget<bwCheckbox>("Wireframes");

  builder.setActiveLayout(*panel);
  builder.addWidget<bwTextBox>().setText("Some Text...");

  builder.addLayout<RowLayout>(false);
  builder.addWidget<bwLabel>("Pose Icon").setIcon(icon_map->getIcon(ICON_POSE_HLT));
  builder.addWidget<bwLabel>("Normalized FCurve Icon")
      .setIcon(icon_map->getIcon(ICON_NORMALIZE_FCURVES));
  builder.addWidget<bwLabel>("Chroma Scope Icon")
      .setIcon(icon_map->getIcon(ICON_SEQ_CHROMA_SCOPE));
}

auto isUseCSSVersionToggleHidden(const bwStyle& style) -> bool
{
  return (style.type_id != bwStyle::TypeID::CLASSIC) &&
         (style.type_id != bwStyle::TypeID::CLASSIC_CSS);
}

void DefaultStage::addStyleSelector(bwScreenGraph::LayoutNode& parent_node)
{
  RNAScreenGraphBuilder<DefaultStage> builder(parent_node, *this, properties);
  using namespace bwScreenGraph;

  builder.addLayout<RowLayout>(true);

  auto& label = builder.addWidget<bwLabel>("Style:");
  label.setIcon(icon_map->getIcon(ICON_BLENDER));

  for (const bwStyle::StyleType& type : bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
    if (type.type_id == bwStyle::TypeID::CLASSIC_CSS) {
      // A checkbox button is added for this variation later.
      continue;
    }
    auto& style_button = builder.addRNAWidget<bwRadioButton>(
        int(type.type_id), "style_type", type.name);

    if (type.type_id == style->type_id) {
      style_button.setState(bwAbstractButton::State::SUNKEN);
    }
  }

  builder.setActiveLayout(parent_node);
  auto& checkbox = builder.addRNAWidget<bwCheckbox>("style_use_css_version", "Use CSS Version");
  checkbox.hide(isUseCSSVersionToggleHidden(*style));
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

auto DefaultStage::updateStyleButton(bwWidget& widget_iter) -> bool
{
  bwStyle::TypeID active_type_id = DefaultStage::style->type_id;

  if (auto* radio_iter = widget_cast<bwRadioButton>(widget_iter)) {
    if (radio_iter->apply_functor) {
      auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(radio_iter->apply_functor.get());

      if (rna_functor && rna_functor->getPropName() == "style_type") {
        auto prop = rna_functor->getEnumValue();
        if (prop && bwStyle::TypeID(prop.value()) == active_type_id) {
          radio_iter->setState(bwWidget::State::SUNKEN);
        }
        else {
          radio_iter->setState(bwWidget::State::NORMAL);
        }
      }
    }
  }
  else if (auto* checkbox_iter = widget_cast<bwCheckbox>(widget_iter)) {
    if (checkbox_iter->apply_functor) {
      auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(
          checkbox_iter->apply_functor.get());
      if (rna_functor && rna_functor->getPropName() == "style_use_css_version") {
        if (active_type_id == bwStyle::TypeID::CLASSIC ||
            active_type_id == bwStyle::TypeID::CLASSIC_CSS) {
          useStyleCSSVersionSet(checkbox_iter->getState() == bwWidget::State::SUNKEN);
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
    if (bwWidget* widget = node.Widget()) {
      updateStyleButton(*widget);
    }
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

    const auto* checkbox = widget_cast<bwCheckbox>(widget);
    if (checkbox && checkbox->apply_functor) {
      const auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(
          checkbox->apply_functor.get());
      if (rna_functor && (rna_functor->getPropName() == "style_use_css_version")) {
        widget->hide(isUseCSSVersionToggleHidden(*Stage::style));
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
    if (auto* iter_checkbox = widget_cast<bwCheckbox>(widget)) {
      if (iter_checkbox->apply_functor) {
        const auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(
            iter_checkbox->apply_functor.get());
        if (rna_functor && (rna_functor->getPropName() == "font_use_subpixel_positioning")) {
          iter_checkbox->hide(!value);
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
