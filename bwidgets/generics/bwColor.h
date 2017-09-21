#pragma once

namespace bWidgets {

class bwColor
{
public:
	bwColor(float red, float green, float blue, float alpha = 1.0f);
	bwColor(float rgb = 0.0f, float alpha = 1.0f);

	bwColor& shade(const float rgb_shade, float alpha_shade = 0.0f);

	void setColor(float red, float green, float blue, float alpha = 1.0f);
	void setColor(float rgb, float alpha = 1.0f);
	void setColor(const float rgba[4]);
	const float* getColor() const;
	void operator=(const float rgb);

	bool operator==(const bwColor& compare_color) const;
	float& operator[](unsigned int index);
	// Implicit conversion to float*
	operator const float*() const;

private:
	float rgba[4];
};

} // bWidgets
