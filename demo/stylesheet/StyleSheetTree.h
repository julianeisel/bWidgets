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

#include <unordered_map>

#include "bwWidget.h"

namespace bWidgetsDemo {

class StyleSheetTree {
 public:
  ~StyleSheetTree();

  bWidgets::bwStyleProperty& ensureNodeWithProperty(const std::string& class_name,
                                                    const bWidgets::bwWidget::State state,
                                                    const std::string& identifier,
                                                    const bWidgets::bwStyleProperty::Type type);

  class StyleSheetNode& ensureNode(const std::string& class_name);

  const bWidgets::bwStyleProperty* resolveProperty(const std::string& class_name,
                                                   const std::string& property_name,
                                                   const bWidgets::bwWidget::State state);

 private:
  class StyleSheetNode* lookupNode(const std::string& name);

  std::unordered_map<std::string, class StyleSheetNode*> nodes{0};
};

}  // namespace bWidgetsDemo
