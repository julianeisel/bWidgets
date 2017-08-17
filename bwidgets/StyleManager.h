#pragma once

#include <array>

#include "Style.h"

namespace bWidgets {

/**
 * \brief Singleton class for managing styles through type ID's.
 */
class StyleManager
{
public:
	static StyleManager& getStyleManager();
	static Style* createStyleFromTypeID(Style::StyleTypeID type_id);

	void registerDefaultStyleTypes();

	typedef std::array<Style::StyleType, Style::STYLE_BUILTIN_TOT> StyleTypeArray;
	const StyleTypeArray& getBuiltinStyleTypes() const;

private:
	StyleManager();
	StyleManager(StyleManager const&) = delete;
	void operator=(StyleManager const&) = delete;

	StyleTypeArray builtin_style_types;
//	std::vector<StyleType> custom_types;
};

}
