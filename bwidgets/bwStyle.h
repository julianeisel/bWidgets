#pragma once

#include <array>
#include <string>

namespace bWidgets {

class bwStyle {
 public:
  enum class TypeID {
    CLASSIC,
    CLASSIC_CSS,
    FLAT_GREY,
    FLAT_DARK,
    FLAT_LIGHT,

    BUILTIN_TOT,

    //		STYLE_CUSTOM, // For the future
  } type_id;

  virtual ~bwStyle() = default;

  virtual void setWidgetStyle(class bwWidget& widget) = 0;
  virtual void polish(class bwWidget& widget) = 0;

  static unsigned int s_default_widget_size_hint;

  float dpi_fac{1.0f};

  struct StyleType {
    TypeID type_id;
    std::string name;
  };

 protected:
  bwStyle(TypeID type_id);
};

}  // namespace bWidgets
