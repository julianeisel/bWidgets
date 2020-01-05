#pragma once

#include "bwRectangle.h"

namespace bWidgets {

class bwLayoutInterface {
 public:
  virtual ~bwLayoutInterface() = default;

  virtual bwRectanglePixel getRectangle() = 0;
};

}  // namespace bWidgets
