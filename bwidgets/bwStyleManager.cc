#include <cassert>

#include "bwStyleClassic.h"
#include "bwStyleFlatGrey.h"
#include "bwStyleFlatLight.h"
#include "bwStyleFlatDark.h"
#include "bwStyleCSS.h"

#include "bwStyleManager.h"

using namespace bWidgets;


bwStyleManager::bwStyleManager()
{
	
}

bwStyleManager& bwStyleManager::getStyleManager()
{
	static bwStyleManager instance;
	return instance;
}

bwPointer<bwStyle> bwStyleManager::createStyleFromTypeID(bwStyle::StyleTypeID type)
{
	switch (type) {
		case bwStyle::STYLE_CLASSIC:
			return bwPointer_new<bwStyleClassic>();
		case bwStyle::STYLE_CLASSIC_CSS:
			return bwPointer_new<bwStyleCSS>();
		case bwStyle::STYLE_FLAT_GREY:
			return bwPointer_new<bwStyleFlat>();
		case bwStyle::STYLE_FLAT_DARK:
			return bwPointer_new<bwStyleFlatDark>();
		case bwStyle::STYLE_FLAT_LIGHT:
			return bwPointer_new<bwStyleFlatLight>();
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
