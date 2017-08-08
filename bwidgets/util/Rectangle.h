#pragma once

namespace bWidgets {

template <typename T>
struct Rectangle
{
	inline Rectangle(const T xmin, const T xmax, const T ymin, const T ymax) :
	    xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
	{
		
	}
	template <typename U>
	inline Rectangle(const Rectangle<U>& rect) :
	    xmin(rect.xmin), xmax(rect.xmax), ymin(rect.ymin), ymax(rect.ymax)
	{
		
	}

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

	inline T centerX() const
	{
		return xmin + ((xmax - xmin) / (T)2);
	}
	inline T centerY() const
	{
		return ymin + ((ymax - ymin) / (T)2);
	}

	T xmin, xmax;
	T ymin, ymax;
};

} // namespace bWidgets
