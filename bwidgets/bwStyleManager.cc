#include <cassert>

#include "bwStyleBlenderClassic.h"
#include "bwStyleBlenderFlat.h"

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

bwStyle* bwStyleManager::createStyleFromTypeID(bwStyle::StyleTypeID type)
{
	switch (type) {
		case bwStyle::STYLE_BLENDER_CLASSIC:
			return new bwStyleBlenderClassic();
		case bwStyle::STYLE_BLENDER_FLAT:
			return new bwStyleBlenderFlat();
		default:
			assert(0);
			return nullptr;
	}
}

void bwStyleManager::registerDefaultStyleTypes()
{
	builtin_style_types[bwStyle::STYLE_BLENDER_CLASSIC].type_id = bwStyle::STYLE_BLENDER_CLASSIC;
	builtin_style_types[bwStyle::STYLE_BLENDER_CLASSIC].name = "Blender Classic";

	builtin_style_types[bwStyle::STYLE_BLENDER_FLAT].type_id = bwStyle::STYLE_BLENDER_FLAT;
	builtin_style_types[bwStyle::STYLE_BLENDER_FLAT].name = "Blender Flat";
}

const bwStyleManager::StyleTypeArray& bwStyleManager::getBuiltinStyleTypes() const
{
	return builtin_style_types;
}
