#pragma once

#include <array>

#include "bwStyle.h"


namespace bWidgets {

/**
 * \brief Singleton class for managing styles through type ID's.
 */
class bwStyleManager
{
public:
	static bwStyleManager& getStyleManager();
	static bwStyle* createStyleFromTypeID(bwStyle::StyleTypeID type_id);

	void registerDefaultStyleTypes();

	typedef std::array<bwStyle::StyleType, bwStyle::STYLE_BUILTIN_TOT> StyleTypeArray;
	const StyleTypeArray& getBuiltinStyleTypes() const;

private:
	bwStyleManager();
	bwStyleManager(bwStyleManager const&) = delete;
	void operator=(bwStyleManager const&) = delete;

	StyleTypeArray builtin_style_types;
//	std::vector<StyleType> custom_types;
};

}
