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
