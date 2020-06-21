#pragma once

#include "bwRectangle.h"

namespace bWidgets {

class bwLayoutInterface {
 public:
  virtual ~bwLayoutInterface() = default;

  virtual auto getRectangle() -> bwRectanglePixel = 0;
};

}  // namespace bWidgets
