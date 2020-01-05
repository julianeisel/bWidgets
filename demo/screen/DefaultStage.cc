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

using namespace bWidgetsDemo;
using namespace bWidgets;  // Less verbose

#define BUTTON_HEIGHT 20
#define PANEL_HEADER_HEIGHT 24

namespace bWidgetsDemo {

class ScaleSetter : public bwFunctorInterface {
 public:
  ScaleSetter(const bwNumberSlider&);
  void operator()() override;

 private:
  const bwNumberSlider& numslider;
};

class StyleSetter : public bwFunctorInterface {
 public:
  StyleSetter(DefaultStage& stage, const bwStyle::StyleType& style_type);
  static bool updateStyleButton(bwWidget&, DefaultStage&);
  void operator()() override;

 private:
  DefaultStage& stage;
  const bwStyle::StyleType& style_type;
};

class UseCSSVersionToggleSetter : public bwFunctorInterface {
 public:
  UseCSSVersionToggleSetter(const bwCheckbox&, DefaultStage&);
  void operator()() override;

 private:
  const bwCheckbox& checkbox;
  DefaultStage& stage;
};

class UseFontTightPlacementToggleSetter : public bwFunctorInterface {
 public:
  UseFontTightPlacementToggleSetter(const bwCheckbox& _checkbox) : checkbox(_checkbox)
  {
  }

  void operator()() override
  {
    Stage::setFontTightPositioning(checkbox.isChecked());
  }

 private:
  const bwCheckbox& checkbox;
};

class UseFontHintingToggleSetter : public bwFunctorInterface {
 public:
  UseFontHintingToggleSetter(const bwCheckbox& _checkbox) : checkbox(_checkbox)
  {
  }

  void operator()() override
  {
    Stage::setFontHinting(checkbox.isChecked());
  }

 private:
  const bwCheckbox& checkbox;
};

class UseFontSubPixelPositioningToggleSetter : public bwFunctorInterface {
 public:
  UseFontSubPixelPositioningToggleSetter(const bwCheckbox& _checkbox) : checkbox(_checkbox)
  {
  }

  void operator()() override
  {
    Stage::setFontSubPixelPositioning(checkbox.isChecked());
  }

 private:
  const bwCheckbox& checkbox;
};

class UseFontSubPixelsToggleSetter : public bwFunctorInterface {
 public:
  UseFontSubPixelsToggleSetter(const bwCheckbox& _checkbox, Stage& _stage)
      : checkbox(_checkbox), stage(_stage)
  {
  }

  void operator()() override
  {
    Stage::setFontAntiAliasingMode(checkbox.isChecked() ? Font::SUBPIXEL_LCD_RGB_COVERAGE :
                                                          Font::NORMAL_COVERAGE);
    for (bwScreenGraph::Node& node : stage.screen_graph.Root()) {
      bwWidget* widget = node.Widget();
      if (!widget) {
        continue;
      }
      if (auto* iter_checkbox = widget_cast<bwCheckbox*>(widget)) {
        if (iter_checkbox->apply_functor && dynamic_cast<UseFontSubPixelPositioningToggleSetter*>(
                                                iter_checkbox->apply_functor.get())) {
          iter_checkbox->hidden = !checkbox.isChecked();
        }
      }
    }
  }

 private:
  const bwCheckbox& checkbox;
  Stage& stage;
};

}  // namespace bWidgetsDemo

// --------------------------------------------------------------------

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height)
    : Stage(mask_width, mask_height)
{
  using namespace bwScreenGraph;
  bwScreenGraph::Builder builder(screen_graph);
  ContainerNode* panel;

  addStyleSelector(screen_graph.Root());

  auto& slider = builder.addWidget<bwNumberSlider>(0, BUTTON_HEIGHT);
  slider.apply_functor = bwPtr_new<ScaleSetter>(slider);
  slider.setText("Interface Scale: ");
  slider.setMinMax(0.5f, 2.0f);
  slider.setValue(1.0f);

  builder.addWidget<bwLabel>("Font Rendering:", 0, BUTTON_HEIGHT);

  builder.addLayout<RowLayout>(true);
  auto* checkbox = &builder.addWidget<bwCheckbox>("Tight Positioning", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<UseFontTightPlacementToggleSetter>(*checkbox);
  checkbox->state = bwWidget::State::SUNKEN;
  checkbox = &builder.addWidget<bwCheckbox>("Hinting", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<UseFontHintingToggleSetter>(*checkbox);

  builder.setActiveLayout(screen_graph.Root());
  builder.addLayout<RowLayout>(false);
  checkbox = &builder.addWidget<bwCheckbox>("Subpixel Rendering", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<UseFontSubPixelsToggleSetter>(*checkbox, *this);
  checkbox = &builder.addWidget<bwCheckbox>("Subpixel Positioning", 0, BUTTON_HEIGHT);
  checkbox->apply_functor = bwPtr_new<UseFontSubPixelPositioningToggleSetter>(*checkbox);
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

void DefaultStage::activateStyleID(bwStyle::TypeID type_id)
{
  Stage::activateStyleID(type_id);
  for (auto& iter_node : screen_graph) {
    bwWidget* widget = iter_node.Widget();
    if (!widget) {
      continue;
    }

    if (auto* checkbox = widget_cast<bwCheckbox*>(widget)) {
      if (checkbox->apply_functor &&
          dynamic_cast<UseCSSVersionToggleSetter*>(checkbox->apply_functor.get())) {
        widget->hidden = isUseCSSVersionToggleHidden(*Stage::style);
      }
    }
  }
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

    style_button.apply_functor = bwPtr_new<StyleSetter>(*this, type);

    if (type.type_id == style->type_id) {
      style_button.state = bwAbstractButton::State::SUNKEN;
    }
  }

  builder.setActiveLayout(parent_node);
  auto& checkbox = builder.addWidget<bwCheckbox>("Use CSS Version", 0, BUTTON_HEIGHT);
  checkbox.hidden = isUseCSSVersionToggleHidden(*style);
  checkbox.apply_functor = bwPtr_new<UseCSSVersionToggleSetter>(checkbox, *this);
}

void DefaultStage::addFakeSpacer(bwScreenGraph::LayoutNode& parent_node)
{
  // Just some extra space. No spacer widgets yet.
  bwScreenGraph::Builder::emplaceWidget<bwLabel>(parent_node, "", 0, 7);
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

// --------------------------------------------------------------------
// Functor definitions

ScaleSetter::ScaleSetter(const bwNumberSlider& numslider) : numslider(numslider)
{
}

void ScaleSetter::operator()()
{
  Stage::setInterfaceScale(numslider.getValue());
}

StyleSetter::StyleSetter(DefaultStage& stage, const bwStyle::StyleType& style_type)
    : stage(stage), style_type(style_type)
{
}

bool StyleSetter::updateStyleButton(bwWidget& widget_iter, DefaultStage& stage)
{
  auto* radio_iter = widget_cast<bwRadioButton*>(&widget_iter);
  bwStyle::TypeID active_type_id = DefaultStage::style->type_id;

  if (radio_iter && radio_iter->apply_functor) {
    // Using dynamic_cast to check if apply_functor is a StyleSetter. Then we assume it's a style
    // button.
    if (auto iter_style_setter = dynamic_cast<StyleSetter*>(radio_iter->apply_functor.get())) {
      if (iter_style_setter->style_type.type_id == active_type_id) {
        radio_iter->state = bwWidget::State::SUNKEN;
      }
      else {
        radio_iter->state = bwWidget::State::NORMAL;
      }
    }
  }
  else if (auto* checkbox_iter = widget_cast<bwCheckbox*>(&widget_iter)) {
    if (checkbox_iter->apply_functor &&
        dynamic_cast<UseCSSVersionToggleSetter*>(checkbox_iter->apply_functor.get())) {
      if (active_type_id == bwStyle::TypeID::CLASSIC ||
          active_type_id == bwStyle::TypeID::CLASSIC_CSS) {
        stage.useStyleCSSVersionSet(checkbox_iter->state == bwWidget::State::SUNKEN);
      }
    }
  }

  return true;
}

void StyleSetter::operator()()
{
  bwStyle::TypeID style_type_id = style_type.type_id;
  stage.activateStyleID(style_type_id);
  // Deactivate other style radio buttons
  for (bwScreenGraph::Node& node : stage.screen_graph) {
    bwWidget* widget = node.Widget();
    if (!widget || widget->hidden) {
      continue;
    }
    updateStyleButton(*widget, stage);
  }
}

UseCSSVersionToggleSetter::UseCSSVersionToggleSetter(const bwCheckbox& checkbox,
                                                     DefaultStage& stage)
    : checkbox(checkbox), stage(stage)
{
}

void UseCSSVersionToggleSetter::operator()()
{
  stage.useStyleCSSVersionSet(checkbox.state == bwWidget::State::SUNKEN);
}
