#pragma once

namespace bWidgets {

template<typename T> class bwRectangle {
 public:
  inline bwRectangle(const T xmin = 0, const T xmax = 0, const T ymin = 0, const T ymax = 0)
      : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
  {
  }
  template<typename U>
  inline bwRectangle(const bwRectangle<U>& rect)
      : xmin(rect.xmin), xmax(rect.xmax), ymin(rect.ymin), ymax(rect.ymax)
  {
  }
  template<typename U>
  inline bwRectangle(bwRectangle<U>&& rect)
      : xmin(rect.xmin), xmax(rect.xmax), ymin(rect.ymin), ymax(rect.ymax)
  {
  }

  inline void set(const T _xmin, const T width, const T _ymin, const T height)
  {
    xmin = _xmin;
    xmax = xmin + width;
    ymin = _ymin;
    ymax = ymin + height;
  }

  /**
   * Resize the rectangle by adding \a pixel to each side.
   */
  inline void resize(const T pixel)
  {
    xmin -= pixel;
    xmax += pixel;
    ymin -= pixel;
    ymax += pixel;
  }

  template<typename U> inline bool isCoordinateInside(const U x, const U y) const
  {
    return (x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax);
  }

  inline bool isEmpty() const
  {
    return (xmin == xmax) || (ymin == ymax);
  }

  inline T width() const
  {
    return xmax - xmin;
  }
  inline T height() const
  {
    return ymax - ymin;
  }

  inline T centerX() const
  {
    return xmin + (width() / T(2));
  }
  inline T centerY() const
  {
    return ymin + (height() / T(2));
  }

  inline void scale(float scale)
  {
    const T cent_x = centerX();
    const T cent_y = centerY();
    const T size_x_half = width() * (scale * 0.5f);
    const T size_y_half = height() * (scale * 0.5f);
    xmin = cent_x - size_x_half;
    ymin = cent_y - size_y_half;
    xmax = cent_x + size_x_half;
    ymax = cent_y + size_y_half;
  }

  inline void clamp(const bwRectangle<T> boundary)
  {
    if (xmin < boundary.xmin) {
      xmin = boundary.xmin;
    }
    if (xmin > boundary.xmax) {
      xmin = boundary.xmax;
    }
    if (xmax > boundary.xmax) {
      xmax = boundary.xmax;
    }
    if (xmax < boundary.xmin) {
      xmax = boundary.xmin;
    }
    if (ymin < boundary.ymin) {
      ymin = boundary.ymin;
    }
    if (ymin > boundary.ymax) {
      ymin = boundary.ymax;
    }
    if (ymax > boundary.ymax) {
      ymax = boundary.ymax;
    }
    if (ymax < boundary.ymin) {
      ymax = boundary.ymin;
    }
  }

  T xmin = 0;
  T xmax = 0;
  T ymin = 0;
  T ymax = 0;
};

// useful aliases
using bwRectanglePixel = bwRectangle<int>;

}  // namespace bWidgets
