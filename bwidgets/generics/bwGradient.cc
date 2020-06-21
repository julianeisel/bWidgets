#include <cassert>
#include <iostream>

#include "bwPoint.h"

#include "bwGradient.h"

namespace bWidgets {

bwGradient::bwGradient(const bwColor& base_color,
                       float shade_begin,
                       float shade_end,
                       Direction direction)
    : direction(direction)
{
  begin = base_color;
  begin.shade(shade_begin);
  end = base_color;
  end.shade(shade_end);
}

auto bwGradient::calcPointColor(const bwPoint& point, const bwRectanglePixel& bounding_box) const
    -> bwColor
{
  const float pos_fac = (direction == bwGradient::Direction::TOP_BOTTOM) ?
                            ((point.y - bounding_box.ymin) / (float)bounding_box.height()) :
                            ((bounding_box.xmax - point.x) / (float)bounding_box.width());
  const float pos_fac_inv = 1.0f - pos_fac;

  assert((pos_fac > -1e-5f) && ((pos_fac - 1.0f) <= 1e-6f));

  return bwColor((begin[0] * pos_fac) + (end[0] * pos_fac_inv),
                 (begin[1] * pos_fac) + (end[1] * pos_fac_inv),
                 (begin[2] * pos_fac) + (end[2] * pos_fac_inv),
                 (begin[3] * pos_fac) + (end[3] * pos_fac_inv));
}

}  // namespace bWidgets
