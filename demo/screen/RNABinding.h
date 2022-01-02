/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <optional>

#include "DefaultStage.h"
#include "data_binding/Binding.h"

#pragma once

namespace bWidgetsDemo {

template<typename RNAObjectType, typename DataType>
class RNABinding : public bWidgets::FunctionalBinding<DataType> {
  RNAObjectType& obj_;
  RNAProperties<RNAObjectType>& properties_;
  std::string prop_name_;
  std::optional<int> enum_value_;

 public:
  RNABinding(RNAObjectType& stage,
             RNAProperties<RNAObjectType>& properties,
             std::string propname,
             std::optional<int> enum_vaue = std::nullopt)
      : obj_(stage), properties_(properties), prop_name_(propname), enum_value_(enum_vaue)
  {
  }

  void set(const DataType& value) override
  {
    properties_.set(prop_name_, obj_, value);
  }

  auto get() const -> DataType override
  {
    return *properties_.template get<DataType>(prop_name_, obj_);
  }

  auto operator==(const bWidgets::FunctionalBinding<DataType>& other) const -> bool override
  {
    const RNABinding& other_binding = dynamic_cast<const RNABinding&>(other);
    return (&obj_ == &other_binding.obj_) && (prop_name_ == other_binding.prop_name_) &&
           (enum_value_ == other_binding.enum_value_);
  }
};

}  // namespace bWidgetsDemo
