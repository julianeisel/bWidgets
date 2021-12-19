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

#pragma once

#include "bwCheckbox.h"
#include "bwFunctorInterface.h"

namespace bWidgetsDemo {

/**
 * Functor for dummy checkbox widgets. That is, widgets for demonstration purposes only, without
 * real data bindings. Without binding this to some dummy data, the state is not properly
 * preserved, because by design state of certain widgets is only a representation of the latest
 * model data state, and not preserved on its own.
 *
 * \note Static booleans, or booleans with persistent storage only.
 */
class DummyCheckboxFunctor : public bWidgets::bwFunctorInterface {
  bool& option_ref_;
  const bWidgets::bwCheckbox& checkbox_;

 public:
  DummyCheckboxFunctor(bool& option_ref, const bWidgets::bwCheckbox& checkbox)
      : option_ref_(option_ref), checkbox_(checkbox)
  {
  }

  void operator()() override
  {
    option_ref_ = checkbox_.isChecked();
  }
  auto matches(const bwFunctorInterface& other_) const -> bool override
  {
    const DummyCheckboxFunctor& other = dynamic_cast<const DummyCheckboxFunctor&>(other_);
    return &option_ref_ == &other.option_ref_;
  }
};

}  // namespace bWidgetsDemo
