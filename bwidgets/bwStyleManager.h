#pragma once

#include <array>

#include "bwStyle.h"

namespace bWidgets {

/**
 * \brief Singleton class for managing styles through type ID's.
 */
class bwStyleManager {
 public:
  using StyleTypeArray = std::array<bwStyle::StyleType, int(bwStyle::TypeID::BUILTIN_TOT)>;

  static auto getStyleManager() -> bwStyleManager&;
  static auto createStyleFromTypeID(bwStyle::TypeID type_id) -> std::unique_ptr<bwStyle>;

  void registerDefaultStyleTypes();

  auto getBuiltinStyleTypes() const -> const StyleTypeArray&;

 private:
  bwStyleManager() = default;
  bwStyleManager(bwStyleManager const&) = delete;

  void operator=(bwStyleManager const&) = delete;

  StyleTypeArray builtin_style_types;
  //	std::vector<StyleType> custom_types;
};

}  // namespace bWidgets
