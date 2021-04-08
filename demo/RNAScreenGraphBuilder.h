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

namespace bWidgetsDemo {

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

}