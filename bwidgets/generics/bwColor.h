#pragma once

namespace bWidgets {

class bwColor
{
public:
	bwColor(float red, float green, float blue, float alpha = 1.0f);
	bwColor(float rgb, float alpha = 1.0f);
	bwColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha = 255);
	bwColor(unsigned int rgb, unsigned int alpha = 255);
	bwColor();

	bwColor& shade(float rgb_shade, float alpha_shade = 0.0f);
	bwColor& shade(unsigned int rgb_shade, unsigned int alpha_shade = 0.0f);

	void setColor(float red, float green, float blue, float alpha = 1.0f);
	void setColor(float rgb, float alpha = 1.0f);
	void setColor(const float rgba[4]);
	const float* getColor() const;
	bwColor& operator=(const float* rgb);
	bwColor& operator=(const bwColor& other_color);

	bool operator==(const bwColor& compare_color) const;
	float& operator[](unsigned int index);
	// Implicit conversion to float*
	operator const float*() const;

private:
	enum Component {
		COMPONENT_RED   = 0,
		COMPONENT_GREEN = 1,
		COMPONENT_BLUE  = 2,
		COMPONENT_ALPHA = 3,
	};
	void clamp(const Component component);

	float rgba[4]{0};
};

} // bWidgets
