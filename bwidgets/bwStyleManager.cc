#include <cassert>

#include "bwStyleClassic.h"
#include "bwStyleFlatDark.h"
#include "bwStyleFlatGrey.h"
#include "bwStyleFlatLight.h"

#include "bwStyleManager.h"

namespace bWidgets {

bwStyleManager& bwStyleManager::getStyleManager()
{
  static bwStyleManager instance;
  return instance;
}

auto bwStyleManager::createStyleFromTypeID(bwStyle::TypeID type) -> std::unique_ptr<bwStyle>
{
  using std::make_unique;

  switch (type) {
    case bwStyle::TypeID::CLASSIC:
      return make_unique<bwStyleClassic>();
    case bwStyle::TypeID::CLASSIC_CSS:
      return make_unique<bwStyleCSS>();
    case bwStyle::TypeID::FLAT_GREY:
      return make_unique<bwStyleFlat>();
    case bwStyle::TypeID::FLAT_DARK:
      return make_unique<bwStyleFlatDark>();
    case bwStyle::TypeID::FLAT_LIGHT:
      return make_unique<bwStyleFlatLight>();
    default:
      assert(0);
      return nullptr;
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

auto bwStyleManager::getBuiltinStyleTypes() const -> const bwStyleManager::StyleTypeArray&
{
  return builtin_style_types;
}

}  // namespace bWidgets
