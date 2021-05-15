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

#include "builtin_widgets.h"

#include "DefaultStageRNAFunctor.h"

using namespace bWidgets;

namespace bWidgetsDemo {

DefaultStageRNAFunctor::DefaultStageRNAFunctor(RNAProperties<DefaultStage>& props,
                                               DefaultStage& stage,
                                               const std::string& prop_name,
                                               const bwWidget& widget)
    : m_props(props), m_stage(stage), m_prop_name(prop_name), m_widget(widget)
{
  /* Sanity check. */
  assert(props.find(prop_name));
}

DefaultStageRNAFunctor::DefaultStageRNAFunctor(RNAProperties<DefaultStage>& props,
                                               DefaultStage& stage,
                                               const std::string& prop_name,
                                               const bwWidget& widget,
                                               int enum_value)
    : DefaultStageRNAFunctor(props, stage, prop_name, widget)
{
  m_enum_value = enum_value;
}

void DefaultStageRNAFunctor::operator()()
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

auto DefaultStageRNAFunctor::operator==(const bWidgets::bwFunctorInterface& other) const -> bool
{
  const DefaultStageRNAFunctor* other_functor = dynamic_cast<const DefaultStageRNAFunctor*>(
      &other);
  if (!other_functor) {
    return false;
  }

  return (&m_stage == &other_functor->m_stage) == (m_prop_name == other_functor->m_prop_name);
}

const std::string& DefaultStageRNAFunctor::getPropName() const
{
  return m_prop_name;
}

std::optional<int> DefaultStageRNAFunctor::getEnumValue() const
{
  return m_enum_value;
}

}  // namespace bWidgetsDemo
