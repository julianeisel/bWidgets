#include <cassert>

#include "bwStyleClassic.h"
#include "bwStyleFlatGrey.h"
#include "bwStyleFlatLight.h"
#include "bwStyleFlatDark.h"

#include "bwStyleManager.h"

namespace bWidgets {

bwStyleManager& bwStyleManager::getStyleManager()
{
  static bwStyleManager instance;
  return instance;
}

bwPtr<bwStyle> bwStyleManager::createStyleFromTypeID(bwStyle::TypeID type)
{
  switch (type) {
    case bwStyle::TypeID::CLASSIC:
      return bwPtr_new<bwStyleClassic>();
    case bwStyle::TypeID::CLASSIC_CSS:
      return bwPtr_new<bwStyleCSS>();
    case bwStyle::TypeID::FLAT_GREY:
      return bwPtr_new<bwStyleFlat>();
    case bwStyle::TypeID::FLAT_DARK:
      return bwPtr_new<bwStyleFlatDark>();
    case bwStyle::TypeID::FLAT_LIGHT:
      return bwPtr_new<bwStyleFlatLight>();
    default:
      assert(0);
  }
}

void bwStyleManager::registerDefaultStyleTypes()
{
  builtin_style_types[int(bwStyle::TypeID::CLASSIC)].type_id = bwStyle::TypeID::CLASSIC;
  builtin_style_types[int(bwStyle::TypeID::CLASSIC)].name = "Classic";

  builtin_style_types[int(bwStyle::TypeID::CLASSIC_CSS)].type_id = bwStyle::TypeID::CLASSIC_CSS;
  builtin_style_types[int(bwStyle::TypeID::CLASSIC_CSS)].name = "Classic (CSS)";

  builtin_style_types[int(bwStyle::TypeID::FLAT_GREY)].type_id = bwStyle::TypeID::FLAT_GREY;
  builtin_style_types[int(bwStyle::TypeID::FLAT_GREY)].name = "Flat Grey";

  builtin_style_types[int(bwStyle::TypeID::FLAT_DARK)].type_id = bwStyle::TypeID::FLAT_DARK;
  builtin_style_types[int(bwStyle::TypeID::FLAT_DARK)].name = "Flat Dark (CSS)";

  builtin_style_types[int(bwStyle::TypeID::FLAT_LIGHT)].type_id = bwStyle::TypeID::FLAT_LIGHT;
  builtin_style_types[int(bwStyle::TypeID::FLAT_LIGHT)].name = "Flat Light (CSS)";
}

const bwStyleManager::StyleTypeArray& bwStyleManager::getBuiltinStyleTypes() const
{
  return builtin_style_types;
}

}  // namespace bWidgets
