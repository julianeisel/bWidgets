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

#pragma once

#include "Stage.h"

namespace bWidgetsDemo {

class DefaultStage : public Stage {
  friend class ScaleSetter;
  friend class StyleSetter;

 public:
  DefaultStage(unsigned int mask_width, unsigned int mask_height);

 private:
  void registerProperties(RNAProperties<DefaultStage>& properties);

  void activateStyleID(bWidgets::bwStyle::TypeID) override;
  void addStyleSelector(bWidgets::bwScreenGraph::LayoutNode& parent_node);
  void updateFontAAMode(bool value);

  void useStyleCSSVersionSet(const bool use_css_version);
  void updateStyleButtons();
  auto updateStyleButton(bWidgets::bwWidget& widget_iter) -> bool;

  RNAProperties<DefaultStage> properties;

  const unsigned int padding = 10;
};

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

}  // namespace bWidgetsDemo
