#pragma once

#include <array>
#include <string>

namespace bWidgets {

class bwStyle {
 public:
  enum StyleTypeID {
    STYLE_CLASSIC,
    STYLE_CLASSIC_CSS,
    STYLE_FLAT_GREY,
    STYLE_FLAT_DARK,
    STYLE_FLAT_LIGHT,

    STYLE_BUILTIN_TOT,

    //		STYLE_CUSTOM, // For the future
  } type_id;

  virtual ~bwStyle()
  {
  }

  virtual void setWidgetStyle(class bwWidget &widget) = 0;
  virtual void polish(class bwWidget &widget) = 0;

  float dpi_fac{1.0f};

  struct StyleType {
    StyleTypeID type_id;
    std::string name;
  };

 protected:
  bwStyle(StyleTypeID type_id);
};

}  // namespace bWidgets
