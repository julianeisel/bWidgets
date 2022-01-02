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
 * Original work Copyright (c) 2020 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#pragma once

#include <memory>

#include "RNAProperty.h"

#include "RNABinding.h"
#include "screen_graph/Builder.h"

namespace bWidgets {
class bwRadioButton;
class bwCheckbox;
}  // namespace bWidgets

namespace bWidgetsDemo {

template<typename _Obj, typename _Func>
class RNAScreenGraphBuilder : public bWidgets::bwScreenGraph::Builder {
 public:
  RNAScreenGraphBuilder(bWidgets::bwScreenGraph::LayoutNode& node,
                        _Obj& obj,
                        RNAProperties<_Obj>& properties)
      : bWidgets::bwScreenGraph::Builder(node), m_obj(obj), m_props(properties)
  {
  }
  RNAScreenGraphBuilder(bWidgets::bwScreenGraph::ScreenGraph& screen_graph,
                        _Obj& obj,
                        RNAProperties<_Obj>& properties)
      : RNAScreenGraphBuilder(*screen_graph.Root(), obj, properties)
  {
    if (!screen_graph.Root()) {
      throw std::exception();
    }
  }

  template<typename _WidgetType, typename... _Args>
  _WidgetType& addRNAWidget(const std::string& propname, _Args&&... __args)
  {
    static_assert(!std::is_same<_WidgetType, bWidgets::bwRadioButton>::value,
                  "RNAScreenGraphBuilder: For bwRadioButton, addRNAWidget overload with enum "
                  "value should be called.");

    if constexpr (bWidgets::has_binding<_WidgetType>::value) {
      auto binding =
          bWidgets::FunctionalBindingNew<RNABinding<_Obj, typename _WidgetType::DataBindingType>>(
              m_obj, m_props, propname);

      _WidgetType& widget = bWidgets::bwScreenGraph::Builder::addWidget<_WidgetType>(
          std::move(binding), std::forward<_Args>(__args)...);
      return widget;
    }
    else {
      _WidgetType& widget = bWidgets::bwScreenGraph::Builder::addWidget<_WidgetType>(
          std::forward<_Args>(__args)...);
      widget.apply_functor = std::make_unique<_Func>(m_props, m_obj, propname, widget);
      return widget;
    }
  }

  template<typename _WidgetType, typename... _Args>
  _WidgetType& addRNAWidget(int enum_value, const std::string& propname, _Args&&... __args)
  {
    _WidgetType& widget = bWidgets::bwScreenGraph::Builder::addWidget<_WidgetType>(
        std::forward<_Args>(__args)...);
    widget.apply_functor = std::make_unique<_Func>(m_props, m_obj, propname, widget, enum_value);
    return widget;
  }

 private:
  _Obj& m_obj;
  RNAProperties<_Obj>& m_props;
};

}  // namespace bWidgetsDemo
