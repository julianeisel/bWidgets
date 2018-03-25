#include "bwStyleProperties.h"
#include "bwWidget.h"

#include "StyleSheetTree.h"


using namespace bWidgetsDemo;
using namespace bWidgets;

namespace bWidgetsDemo {

class StateProperties
{
public:
	const bwStyleProperty* lookupProperty(
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


StyleSheetTree::~StyleSheetTree()
{
	for (auto& node_pair : nodes) { // std::pair
		StyleSheetNode* node = node_pair.second;

		delete node;
	}
}

StyleSheetNode* StyleSheetTree::lookupNode(const std::string& name)
{
	const auto& node_iterator = nodes.find(name);
	return (node_iterator == nodes.end()) ? nullptr : node_iterator->second;
}

StyleSheetNode& StyleSheetTree::ensureNode(
        const std::string& class_name)
{
	StyleSheetNode* node = lookupNode(class_name);

	if (!node) {
		node = new StyleSheetNode;
		nodes.insert({class_name, node});
	}

	return *node;
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

static const bwStyleProperty* state_properties_lookup_property(
        const std::string& property_name,
        StateProperties& state_properties)
{
	return state_properties.lookupProperty(property_name);
}

const bwStyleProperty* StyleSheetTree::resolveProperty(
        const std::string& class_name,
        const std::string& property_name,
        const bwWidget::WidgetState state)
{
	if (StyleSheetNode* node = lookupNode(class_name)) {
		const bwStyleProperty* property = state_properties_lookup_property(
		                                      property_name, node->state_properties[state]);

		if (!property && (state != bwWidget::STATE_NORMAL)) {
			// Property for this state not set, check for STATE_NORMAL.
			property = state_properties_lookup_property(
			               property_name, node->state_properties[bwWidget::STATE_NORMAL]);
		}

		return property;
	}

	return nullptr;
}


const bwStyleProperty* StateProperties::lookupProperty(
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
	for (const auto& iter_property : properties) {
		if (iter_property->getIdentifier() == identifier) {
			return *iter_property;
		}
	}

	return properties.addProperty(identifier, type);
}
