#pragma once

namespace bWidgets {

template<typename T>
class bwRange
{
public:
	bwRange(T _min, T _max) :
	    min(_min), max(_max)
	{
		
	}

	void clampValue(T& value)
	{
		value = (value < min) ? min : ((value > max) ? max : value);
	}
	static void clampValue(T& value, const T& min, const T& max)
	{
		bwRange<T>(min, max).clampValue(value);
	}


	T min, max;
};

} // namespace bWidgets
