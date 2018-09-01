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

#include "bwUtil.h"

#include "katana.h"


namespace bWidgets {
	class bwStyleProperty;
}

namespace bWidgetsDemo {

class StyleSheet
{
public:
	StyleSheet(const std::string& filepath);
	~StyleSheet();

	void reload();

	void resolveValue(
	        const std::string& class_name,
	        bWidgets::bwWidget::WidgetState state,
	        bWidgets::bwStyleProperty& property);

	const std::string& getFilepath() const;

private:
	void load();
	void unload();

	std::string filepath;
	bWidgets::bwPointer<class StyleSheetTree> tree;
};

} // namespace bWidgetsDemo
