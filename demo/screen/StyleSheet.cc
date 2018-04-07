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
#include <fstream>
#include <iostream>

#include "bwColor.h"
#include "bwStyleProperties.h"

#include "File.h"
#include "StyleSheetTree.h"

#include "StyleSheet.h"

using namespace bWidgets;
using namespace bWidgetsDemo;



StyleSheet::StyleSheet(const std::string& filepath) :
    filepath(filepath)
{
	load();
}

StyleSheet::~StyleSheet()
{
	unload();
}

static bwColor stylesheet_get_color_from_katana_value(const KatanaValue& value)
{
	bwColor color{};

	switch (value.unit) {
		case KATANA_VALUE_PARSER_FUNCTION:
		{
			std::string function_name{value.function->name};

			if ((function_name == "rgb(") || (function_name == "rgba(")) {
				for (int i = 0, color_index = 0; i < value.function->args->length; i++) {
					KatanaValue* arg_value = (KatanaValue*)value.function->args->data[i];

					if (arg_value->unit == KATANA_VALUE_PARSER_OPERATOR) {
						continue;
					}
					assert(arg_value->unit == KATANA_VALUE_NUMBER);
					color[color_index++] = arg_value->fValue / 255.0f;
				}
				break;
			}
		}
		// Fall-through
		default:
			assert(!"Only rgb() supported!");
			break;
	}

	return color;
}

static bool stylesheet_get_bool_from_katana_value(const KatanaValue& value)
{
	const std::string ident_value{value.string};

	if (ident_value == "true") {
		return true;
	}
	else if (ident_value == "false") {
		return false;
	}

	assert(0);
	return false;
}

static bwStyleProperty::PropertyType stylesheet_property_type_get_from_katana(
        const KatanaValue& value)
{
	switch (value.unit) {
		case KATANA_VALUE_PARSER_FUNCTION:
		{
			std::string function_name{value.function->name};
			if ((function_name == "rgb(") || (function_name == "rgba(")) {
				return bwStyleProperty::TYPE_COLOR;
			}
			break;
		}
		case KATANA_VALUE_PX:
			return bwStyleProperty::TYPE_FLOAT;
		case KATANA_VALUE_IDENT:
		{
			// Customization to support booleans in CSS.
			const std::string ident_value{value.string};
			assert(ident_value == "true" || ident_value == "false");
			return bwStyleProperty::TYPE_BOOL;
		}
		default:
			return bwStyleProperty::TYPE_INTEGER;
	}

	return bwStyleProperty::TYPE_INTEGER;
}

static void stylesheet_set_value_from_katana_value(
        bwStyleProperty& property,
        const KatanaValue& value)
{
	switch (property.getType()) {
		case bwStyleProperty::TYPE_BOOL:
			property.setValue(stylesheet_get_bool_from_katana_value(value));
			break;
		case bwStyleProperty::TYPE_INTEGER:
			property.setValue((int)value.fValue); // iValue is not valid
			break;
		case bwStyleProperty::TYPE_FLOAT:
			property.setValue((float)value.fValue);
			break;
		case bwStyleProperty::TYPE_COLOR:
			property.setValue(stylesheet_get_color_from_katana_value(value));
			break;
	}
}


static bwWidget::WidgetState stylesheet_property_state_from_katana(
        const KatanaSelector& selector)
{
	KatanaPseudoType pseudo_type = selector.tagHistory ? selector.tagHistory->pseudo : KatanaPseudoEmpty;

	switch (pseudo_type) {
		case KatanaPseudoHover:
			return bwWidget::STATE_HIGHLIGHTED;
		case KatanaPseudoActive:
			return bwWidget::STATE_SUNKEN;
		case KatanaPseudoEmpty:
		default:
			return bwWidget::STATE_NORMAL;
	}
}

static void stylesheet_tree_property_ensure_from_katana(
        StyleSheetTree& tree,
        const KatanaSelector& selector,
        const KatanaDeclaration& declaration,
        const KatanaValue& value)
{
	bwStyleProperty& property = tree.ensureNodeWithProperty(
	                                    selector.tag->local,
	                                    stylesheet_property_state_from_katana(selector),
	                                    declaration.property,
	                                    stylesheet_property_type_get_from_katana(value));
	stylesheet_set_value_from_katana_value(property, value);
}

static void stylesheet_tree_node_fill_from_katana(
        StyleSheetTree& tree,
        const KatanaStyleRule& rule,
        const KatanaSelector& selector)
{
	for (int declaration_idx = 0; declaration_idx < rule.declarations->length; declaration_idx++) {
		KatanaDeclaration* declaration = (KatanaDeclaration*)rule.declarations->data[declaration_idx];

		for (int value_idx = 0; value_idx < declaration->values->length; value_idx++) {
			KatanaValue* value = (KatanaValue*)declaration->values->data[value_idx];
			stylesheet_tree_property_ensure_from_katana(tree, selector, *declaration, *value);
		}
	}
}

static void stylesheet_tree_fill_from_katana(
        StyleSheetTree& tree,
        const KatanaOutput &katana_output)
{
	for (int rule_idx = 0; rule_idx < katana_output.stylesheet->rules.length; rule_idx++) {
		const KatanaStyleRule* rule = (KatanaStyleRule*)katana_output.stylesheet->rules.data[rule_idx];

		for (int selector_idx = 0; selector_idx < rule->selectors->length; selector_idx++) {
			KatanaSelector* selector = (KatanaSelector*)rule->selectors->data[selector_idx];
			stylesheet_tree_node_fill_from_katana(tree, *rule, *selector);
		}
	}
}

void StyleSheet::load()
{
	File file{filepath};
	std::string file_contents = file.readIntoString();
	KatanaOutput* katana_output = katana_parse(
	                                  file_contents.c_str(), file_contents.length(),
	                                  KatanaParserModeStylesheet);

	tree = bwPointer_new<StyleSheetTree>();
	stylesheet_tree_fill_from_katana(*tree, *katana_output);
	katana_destroy_output(katana_output);
}

void StyleSheet::unload()
{
	// Nothing right now.
}

void StyleSheet::reload()
{
	unload();
	load();
}

void StyleSheet::resolveValue(
        const std::string& class_name,
        const bwWidget::WidgetState state,
        bwStyleProperty& property)
{
	const bwStyleProperty* property_from_tree = tree->resolveProperty(class_name, property.getIdentifier(), state);

	if (property_from_tree) {
		property.setValue(*property_from_tree);
	}
	else {
		property.setValueToDefault();
//		std::cout << filepath << std::endl;
//		std::cout << "CSS resolving error: Failed to find property \"" << property.getIdentifier() <<
//		             "\" for " << class_name << "." << std::endl << std::endl;
	}
}

const std::string&StyleSheet::getFilepath() const
{
	return filepath;
}
