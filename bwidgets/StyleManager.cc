#include <cassert>

#include "Style.h"
#include "StyleBlenderClassic.h"
#include "StyleBlenderFlat.h"


#include "StyleManager.h"

using namespace bWidgets;


StyleManager::StyleManager()
{
	
}

StyleManager& StyleManager::getStyleManager()
{
	static StyleManager instance;
	return instance;
}

Style* StyleManager::createStyleFromTypeID(Style::StyleTypeID type)
{
	switch (type) {
		case Style::STYLE_BLENDER_CLASSIC:
			return new StyleBlenderClassic();
		case Style::STYLE_BLENDER_FLAT:
			return new StyleBlenderFlat();
		default:
			assert(0);
			return nullptr;
	}
}

void StyleManager::registerDefaultStyleTypes()
{
	builtin_style_types[Style::STYLE_BLENDER_CLASSIC].type_id = Style::STYLE_BLENDER_CLASSIC;
	builtin_style_types[Style::STYLE_BLENDER_CLASSIC].name = "Blender Classic";

	builtin_style_types[Style::STYLE_BLENDER_FLAT].type_id = Style::STYLE_BLENDER_FLAT;
	builtin_style_types[Style::STYLE_BLENDER_FLAT].name = "Blender Flat";
}

const StyleManager::StyleTypeArray& StyleManager::getBuiltinStyleTypes() const
{
	return builtin_style_types;
}
