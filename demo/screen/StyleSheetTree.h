#pragma once

#include <unordered_map>

#include "bwWidget.h"


namespace bWidgetsDemo {

class StyleSheetTree
{
public:
	~StyleSheetTree();

	bWidgets::bwStyleProperty& ensureNodeWithProperty(
	        const std::string& class_name,
	        const bWidgets::bwWidget::WidgetState state,
	        const std::string& identifier,
	        const bWidgets::bwStyleProperty::PropertyType type);
	class StyleSheetNode& ensureNode(
	        const std::string& class_name);
	const bWidgets::bwStyleProperty* resolveProperty(
	        const std::string& class_name,
	        const std::string& property_name,
	        const bWidgets::bwWidget::WidgetState state);

private:
	class StyleSheetNode* lookupNode(const std::string& name);

	std::unordered_map<std::string, class StyleSheetNode*> nodes{0};
};

} // namespace bWidgetsDemo
