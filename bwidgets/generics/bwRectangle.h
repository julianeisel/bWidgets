#pragma once

namespace bWidgets {

template <typename T>
struct bwRectangle
{
	inline bwRectangle(const T xmin = 0, const T xmax = 0, const T ymin = 0, const T ymax = 0) :
	    xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
	{
		
	}
	template <typename U>
	inline bwRectangle(const bwRectangle<U>& rect) :
	    xmin(rect.xmin), xmax(rect.xmax), ymin(rect.ymin), ymax(rect.ymax)
	{
		
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

	inline bool isCoordinateInside(const T x, const T y) const
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
		return xmin + (width() / (T)2);
	}
	inline T centerY() const
	{
		return ymin + (height() / (T)2);
	}

	T xmin, xmax;
	T ymin, ymax;
};

// useful aliases
using bwRectanglePixel = bwRectangle<int>;

} // namespace bWidgets
