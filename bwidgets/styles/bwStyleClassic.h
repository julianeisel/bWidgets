#pragma once

#include "bwStyle.h"

namespace bWidgets {

class bwStyleClassic : public bwStyle {
 public:
  bwStyleClassic();

  void setWidgetStyle(class bwWidget &widget) override;
  void polish(bwWidget &) override
  {
  }
};

}  // namespace bWidgets
