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

#include <cassert>
#include <string>

#include "katana.h"

#include "PropertyParser.h"

using namespace bWidgets;
using namespace bWidgetsDemo;


bwPointer<PropertyParser> PropertyParser::newFromPropertyType(bWidgets::bwStyleProperty::PropertyType type)
{
	switch (type) {
		case bwStyleProperty::TYPE_BOOL:
			return bwPointer_new<BooleanPropertyParser>();
		case bwStyleProperty::TYPE_INTEGER:
			return bwPointer_new<IntegerPropertyParser>();
		case bwStyleProperty::TYPE_FLOAT:
			return bwPointer_new<FloatPropertyParser>();
		case bwStyleProperty::TYPE_COLOR:
			return bwPointer_new<ColorPropertyParser>();
		default:
			return (assert(0), nullptr);
	}
}

void BooleanPropertyParser::parseIntoProperty(
        bwStyleProperty& dest_property,
        const KatanaValue& value) const
{
	const std::string ident_value{value.string};

	if (ident_value == "true") {
		dest_property.setValue(true);
	}
	else if (ident_value == "false") {
		dest_property.setValue(false);
	}
	else {
		throw property_parsing_failure();
	}
}

void IntegerPropertyParser::parseIntoProperty(
        bwStyleProperty& dest_property,
        const KatanaValue& value) const
{
	dest_property.setValue((int)value.fValue); // iValue is not valid
}

void FloatPropertyParser::parseIntoProperty(
        bwStyleProperty& dest_property,
        const KatanaValue& value) const
{
	dest_property.setValue((float)value.fValue);
}

bool ColorPropertyParser::canParseFunction(const std::string& function_name) const
{
	return (function_name == "rgb(") || (function_name == "rgba(");
}

bwColor ColorPropertyParser::parseFromFunction(const KatanaValue& value) const
{
	if (!canParseFunction(value.function->name)) {
		throw property_parsing_failure();
	}

	{
		bwColor color;

		for (int i = 0, color_index = 0; i < value.function->args->length; i++) {
			KatanaValue *arg_value = (KatanaValue *)value.function->args->data[i];

			if (arg_value->unit == KATANA_VALUE_PARSER_OPERATOR) {
				continue;
			}
			assert(arg_value->unit == KATANA_VALUE_NUMBER);
			color[color_index++] = arg_value->fValue / 255.0f;
		}

		return color;
	}
}

void ColorPropertyParser::parseIntoProperty(
        bwStyleProperty& dest_property,
        const KatanaValue& value) const
{
	switch (value.unit) {
		case KATANA_VALUE_PARSER_FUNCTION:
			dest_property.setValue(parseFromFunction(value));
			break;
		default:
			throw property_parsing_failure();
	}
}