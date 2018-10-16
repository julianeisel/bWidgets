#include <cassert>

#include "bwStyleClassic.h"
#include "bwStyleFlatGrey.h"
#include "bwStyleFlatLight.h"
#include "bwStyleFlatDark.h"

#include "bwStyleManager.h"

using namespace bWidgets;


bwStyleManager& bwStyleManager::getStyleManager()
{
	static bwStyleManager instance;
	return instance;
}

bwPtr<bwStyle> bwStyleManager::createStyleFromTypeID(bwStyle::StyleTypeID type)
{
	switch (type) {
		case bwStyle::STYLE_CLASSIC:
			return bwPtr_new<bwStyleClassic>();
		case bwStyle::STYLE_CLASSIC_CSS:
			return bwPtr_new<bwStyleCSS>();
		case bwStyle::STYLE_FLAT_GREY:
			return bwPtr_new<bwStyleFlat>();
		case bwStyle::STYLE_FLAT_DARK:
			return bwPtr_new<bwStyleFlatDark>();
		case bwStyle::STYLE_FLAT_LIGHT:
			return bwPtr_new<bwStyleFlatLight>();
		default:
			assert(0);
	}
}

void bwStyleManager::registerDefaultStyleTypes()
{
	builtin_style_types[bwStyle::STYLE_CLASSIC].type_id = bwStyle::STYLE_CLASSIC;
	builtin_style_types[bwStyle::STYLE_CLASSIC].name = "Classic";

	builtin_style_types[bwStyle::STYLE_CLASSIC_CSS].type_id = bwStyle::STYLE_CLASSIC_CSS;
	builtin_style_types[bwStyle::STYLE_CLASSIC_CSS].name = "Classic (CSS)";

	builtin_style_types[bwStyle::STYLE_FLAT_GREY].type_id = bwStyle::STYLE_FLAT_GREY;
	builtin_style_types[bwStyle::STYLE_FLAT_GREY].name = "Flat Grey";

	builtin_style_types[bwStyle::STYLE_FLAT_DARK].type_id = bwStyle::STYLE_FLAT_DARK;
	builtin_style_types[bwStyle::STYLE_FLAT_DARK].name = "Flat Dark (CSS)";

	builtin_style_types[bwStyle::STYLE_FLAT_LIGHT].type_id = bwStyle::STYLE_FLAT_LIGHT;
	builtin_style_types[bwStyle::STYLE_FLAT_LIGHT].name = "Flat Light (CSS)";
}

const bwStyleManager::StyleTypeArray& bwStyleManager::getBuiltinStyleTypes() const
{
	return builtin_style_types;
}
