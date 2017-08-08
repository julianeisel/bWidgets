#pragma once

namespace bWidgets {

class Color
{
public:
	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);

	void shade(const float rgb_shade, float alpha_shade = 0.0f);

	void setColor(float red, float green, float blue, float alpha);
	void setColor(const float rgba[4]);
	const float* getColor() const;

	float& operator[](unsigned int index);
	// Implicit conversion to float*
	operator const float*() const;

private:
	float rgba[4];
};

} // bWidgets
