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

#include "StyleSheetTree.h"


using namespace bWidgetsDemo;
using namespace bWidgets;

namespace bWidgetsDemo {

class StateProperties
{
public:
	bwOptional<std::reference_wrapper<const bwStyleProperty>> lookupProperty(
	        const std::string& identifier) const;
	bwStyleProperty& ensureProperty(
	        const std::string& identifier,
	        const bwStyleProperty::PropertyType type);

private:
	bwStyleProperties properties;
};

class StyleSheetNode
{
public:
	~StyleSheetNode() = default;
	class StateProperties state_properties[bwWidget::STATE_TOT];
};

} // namespace bWidgetsDemo


bwOptional<std::reference_wrapper<StyleSheetNode>> StyleSheetTree::lookupNode(const std::string& name)
{
	const auto& node_iterator = nodes.find(name);
	if (node_iterator == nodes.end()) {
		return nullopt;
	}
	else {
		return *node_iterator->second;
	}
}

StyleSheetNode& StyleSheetTree::ensureNode(
        const std::string& class_name)
{
	if (bwOptional<std::reference_wrapper<StyleSheetNode>> node = lookupNode(class_name)) {
		return *node;
	}
	else {
		StyleSheetNode* new_node = new StyleSheetNode;
		nodes.insert({class_name, new_node});
		return *new_node;
	}
}

StyleSheetTree::~StyleSheetTree()
{
	while (!nodes.empty()) {
		auto iterator = nodes.begin();
		StyleSheetNode* node = iterator->second;

		nodes.erase(iterator);
		delete node;
	}
}

bwStyleProperty& StyleSheetTree::ensureNodeWithProperty(
        const std::string& class_name,
        const bwWidget::WidgetState pseudo_state,
        const std::string& identifier,
        const bwStyleProperty::PropertyType type)
{
	StyleSheetNode& node = ensureNode(class_name);
	StateProperties& state_properties = node.state_properties[pseudo_state];

	return state_properties.ensureProperty(identifier, type);
}

static bwOptional<std::reference_wrapper<const bwStyleProperty>> state_properties_lookup_property(
        const std::string& property_name,
        StateProperties& state_properties)
{
	return state_properties.lookupProperty(property_name);
}

bwOptional<std::reference_wrapper<const bwStyleProperty>> StyleSheetTree::resolveProperty(
        const std::string& class_name,
        const std::string& property_name,
        const bwWidget::WidgetState state)
{
	if (bwOptional<std::reference_wrapper<StyleSheetNode>> node = lookupNode(class_name)) {
		bwOptional<std::reference_wrapper<const bwStyleProperty>> property = state_properties_lookup_property(
		                                                                         property_name,
		                                                                         node->get().state_properties[state]);

		if (!property && (state != bwWidget::STATE_NORMAL)) {
			// Property for this state not set, check for STATE_NORMAL.
			property = state_properties_lookup_property(
			               property_name, node->get().state_properties[bwWidget::STATE_NORMAL]);
		}

		return property;
	}

	return nullopt;
}


bwOptional<std::reference_wrapper<const bwStyleProperty>> StateProperties::lookupProperty(
        const std::string& identifier) const
{
	return properties.lookup(identifier);
}

/**
 * Performs a identifier based lookup of \a property and adds it if not found.
 */
bwStyleProperty& StateProperties::ensureProperty(
        const std::string& identifier,
        const bwStyleProperty::PropertyType type)
{
	for (auto& iter_property : properties) {
		if (iter_property->getIdentifier() == identifier) {
			return *iter_property;
		}
	}

	return properties.addProperty(identifier, type);
}
