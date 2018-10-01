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

#include "bwStyleProperties.h"

struct KatanaValue;


namespace bWidgetsDemo {

class property_parsing_failure : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "could not parse value for a property";
	}
};


class PropertyParser
{
public:
	static bWidgets::bwPtr<PropertyParser> newFromPropertyType(bWidgets::bwStyleProperty::PropertyType);

	virtual void parseIntoProperty(
	        bWidgets::bwStyleProperty&,
	        const KatanaValue&) const = 0;
};

class BooleanPropertyParser : public PropertyParser
{
public:
	void parseIntoProperty(
	        bWidgets::bwStyleProperty&,
	        const KatanaValue&) const override;
};

class IntegerPropertyParser : public PropertyParser
{
public:
	void parseIntoProperty(
	        bWidgets::bwStyleProperty&,
	        const KatanaValue&) const override;
};

class FloatPropertyParser : public PropertyParser
{
public:
	void parseIntoProperty(
	        bWidgets::bwStyleProperty&,
	        const KatanaValue&) const override;
};

class ColorPropertyParser : public PropertyParser
{
public:
	void parseIntoProperty(
	        bWidgets::bwStyleProperty&,
	        const KatanaValue&) const override;

private:
	bWidgets::bwColor parseFromFunction(const KatanaValue&) const;
	bool canParseFunction(const std::string&) const;
};

} // namespace bWidgetsDemo
