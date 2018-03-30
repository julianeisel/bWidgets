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

	bool isInside(const T& value, const bool including_min_max = false) const
	{
		if (including_min_max) {
			return (value >= min) && (value <= max);
		}
		else {
			return (value > min) && (value < max);
		}
	}
	static bool isInside(const T& value, const T& min, const T&max, const bool including_min_max = false)
	{
		return bwRange<T>(min, max).isInside(value, including_min_max);
	}


	T min, max;
};

} // namespace bWidgets
