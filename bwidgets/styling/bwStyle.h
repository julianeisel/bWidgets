#pragma once

#include <array>
#include <string>

namespace bWidgets {

class bwWidget;

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
  };

  struct StyleType {
    TypeID type_id;
    std::string name;
  };

  virtual ~bwStyle() = default;

  virtual void setWidgetStyle(bwWidget& widget) = 0;
  virtual void polish(bwWidget&);

  static unsigned int s_default_widget_size_hint;

  TypeID type_id;
  float dpi_fac{1.0f};

 protected:
  bwStyle(TypeID type_id);
};

}  // namespace bWidgets
