#pragma once

#include <array>

#include "bwStyle.h"
#include "bwUtil.h"

namespace bWidgets {

/**
 * \brief Singleton class for managing styles through type ID's.
 */
class bwStyleManager {
 public:
  static bwStyleManager& getStyleManager();
  static bwPtr<bwStyle> createStyleFromTypeID(bwStyle::TypeID type_id);

  void registerDefaultStyleTypes();

  using StyleTypeArray = std::array<bwStyle::StyleType, int(bwStyle::TypeID::BUILTIN_TOT)>;
  const StyleTypeArray& getBuiltinStyleTypes() const;

 private:
  bwStyleManager() = default;
  bwStyleManager(bwStyleManager const&) = delete;

  void operator=(bwStyleManager const&) = delete;
  StyleTypeArray builtin_style_types;
  //	std::vector<StyleType> custom_types;
};

}  // namespace bWidgets
