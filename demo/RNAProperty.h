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

#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <string>

#include "bwUtil.h"

namespace bWidgetsDemo {

class RNAProperty {
 public:
  virtual ~RNAProperty() = default;

 protected:
  RNAProperty() = default;
};

template<typename _Obj, typename _T> using Getter = std::function<_T(const _Obj&)>;
template<typename _Obj, typename _T> using Setter = std::function<void(_Obj&, _T)>;

template<typename _Obj, typename _T> class RNAPropertyInternal : public RNAProperty {
  union Value {
    struct Callbacks {
      const Getter<_Obj, _T> getter;
      const Setter<_Obj, _T> setter;
    } m_callbacks;
    std::reference_wrapper<_T> m_ref;

    Value(_T& ref) : m_ref(ref)
    {
    }
    Value(Callbacks callbacks) : m_callbacks(callbacks)
    {
    }
    ~Value()
    {
    }
  } m_value;

 public:
  RNAPropertyInternal(_T& ref) : m_value(ref), m_use_ref(true)
  {
  }

  RNAPropertyInternal(Getter<_Obj, _T> get, Setter<_Obj, _T> set)
      : m_value({get, set}), m_use_ref(false)
  {
  }

  ~RNAPropertyInternal()
  {
    if (m_use_ref) {
      m_value.m_ref.get().~_T();
    }
    else {
      m_value.m_callbacks.~Callbacks();
    }
  }

  _T get(const _Obj& object) const
  {
    return m_use_ref ? m_value.m_ref.get() : m_value.m_callbacks.getter(object);
  }

  void set(_Obj& object, const _T& value)
  {
    if (m_use_ref) {
      m_value.m_ref.get() = value;
    }
    else {
      m_value.m_callbacks.setter(object, value);
    }
  }

 private:
  bool m_use_ref;
};

template<typename _Obj> class RNAProperties {
 public:
  using ObjectT = _Obj;

  RNAProperties() = default;
  RNAProperties(std::initializer_list<RNAProperty&>);

  template<typename _T>
  RNAProperty& defProperty(const std::string& name,
                           Getter<ObjectT, _T> get,
                           Setter<ObjectT, _T> set)
  {
    auto pair = m_properties.emplace(name,
                                     std::make_unique<RNAPropertyInternal<ObjectT, _T>>(get, set));
    return *(*pair.first).second;
  }

  RNAProperty* find(const std::string& name, bool silent = false) const
  {
    auto iter = m_properties.find(name);
    if (iter != m_properties.end()) {
      return (*iter).second.get();
    }
    if (!silent) {
      std::cout << "RNA Property not found: " << name << std::endl;
      assert(false);
    }
    return nullptr;
  }

  template<typename _T> std::optional<_T> get(const std::string& name, const _Obj& object) const
  {
    if (RNAProperty* prop = find(name)) {
      auto& prop_internal = dynamic_cast<RNAPropertyInternal<ObjectT, _T>&>(*prop);
      return prop_internal.get(object);
    }
    return std::nullopt;
  }
  template<typename _T> void set(const std::string& name, _Obj& object, const _T& value)
  {
    if (RNAProperty* prop = find(name)) {
      auto& prop_internal = static_cast<RNAPropertyInternal<ObjectT, _T>&>(*prop);
      prop_internal.set(object, value);
    }
  }

  std::map<std::string, std::unique_ptr<RNAProperty>> m_properties;
};

#if 0
template<typename _Obj, typename _T>
RNAProperty& RNA_def_property(RNAProperties<_Obj>& properties,
                              const std::string& name,
                              _T& property_ref)
{
  auto pair = properties.m_properties.emplace(
      name, std::make_unique<RNAPropertyInternal<_Obj, _T>>(property_ref));
  return *(*pair.first).second;
}

template<typename _Obj, typename _T>
RNAProperty& RNA_def_property(RNAProperties<_Obj>& properties,
                              const std::string& name,
                              Getter<_Obj, _T> get,
                              Setter<_Obj, _T> set)
{
  auto pair = properties.m_properties.emplace(
      name, std::make_unique<RNAPropertyInternal<_Obj, _T>>(get, set));
  return *(*pair.first).second;
}

template<typename _Obj, typename _T> _T& RNA_property_get(const RNAProperty& prop, _Obj& object)
{
  auto prop_internal = dynamic_cast<RNAPropertyInternal<_Obj, _T>>(prop);
  return prop_internal.get(object);
}
#endif

}  // namespace bWidgetsDemo
