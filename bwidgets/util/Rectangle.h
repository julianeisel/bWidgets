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

	T xmin, xmax;
	T ymin, ymax;
};

} // namespace bWidgets
