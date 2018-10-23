#include <cassert>
#include <cmath>

#include "bwRange.h"

#include "bwColor.h"


using namespace bWidgets;

bwColor::bwColor(
        float red,
        float green,
        float blue,
        float alpha)
{
	setColor(red, green, blue, alpha);
}
bwColor::bwColor(
        float rgb,
        float alpha)
{
	setColor(rgb, alpha);
}
bwColor::bwColor(
        unsigned int red,
        unsigned int green,
        unsigned int blue,
        unsigned int alpha)
{
	assert(red < 256);
	assert(green < 256);
	assert(blue < 256);
	assert(alpha < 256);

	setColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}
bwColor::bwColor(
        unsigned int rgb,
        unsigned int alpha)
{
	assert(rgb < 256);
	assert(alpha < 256);

	setColor(rgb / 255.0f, alpha / 255.0f);
}

bwColor::bwColor()
{
	setColor(0.0f);
}

bwColor& bwColor::shade(const float rgb_shade, float alpha_shade)
{
	rgba[COMPONENT_RED] += rgb_shade;
	clamp(COMPONENT_RED);
	rgba[COMPONENT_GREEN] += rgb_shade;
	clamp(COMPONENT_GREEN);
	rgba[COMPONENT_BLUE] += rgb_shade;
	clamp(COMPONENT_BLUE);
	rgba[COMPONENT_ALPHA] += alpha_shade;

	return *this;
}
bwColor& bwColor::shade(unsigned int rgb_shade, unsigned int alpha_shade)
{
	rgba[COMPONENT_RED] += rgb_shade / 255.0f;
	clamp(COMPONENT_RED);
	rgba[COMPONENT_GREEN] += rgb_shade / 255.0f;
	clamp(COMPONENT_GREEN);
	rgba[COMPONENT_BLUE] += rgb_shade / 255.0f;
	clamp(COMPONENT_BLUE);
	rgba[COMPONENT_ALPHA] += alpha_shade / 255.0f;
	clamp(COMPONENT_ALPHA);

	return *this;
}

void bwColor::setColor(float red, float green, float blue, float alpha)
{
	const float rgba[4] = {red, green, blue, alpha};
	setColor(rgba);
}

void bwColor::setColor(float rgb, float alpha)
{
	const float rgba[4] = {rgb, rgb, rgb, alpha};
	setColor(rgba);
}

void bwColor::setColor(const float _rgba[4])
{
	rgba[0] = _rgba[0];
	rgba[1] = _rgba[1];
	rgba[2] = _rgba[2];
	rgba[3] = _rgba[3];
}

const float* bwColor::getColor() const
{
	return &rgba[0];
}

bwColor& bwColor::operator=(const float* rgb)
{
	setColor(rgb);
	return *this;
}
bwColor& bwColor::operator=(const bwColor& other_color)
{
	setColor(other_color.rgba);
	return *this;
}

bool bwColor::operator==(const bwColor& compare_color) const
{
	return ((rgba[0] == compare_color[0]) &&
	        (rgba[1] == compare_color[1]) &&
	        (rgba[2] == compare_color[2]) &&
	        (rgba[3] == compare_color[3]));
}

float& bwColor::operator[](size_t index)
{
	assert((index >= 0) && (index < 4));
	return rgba[index];
}

void bwColor::clamp(const Component component)
{
	bwRange<float>::clampValue(rgba[component], 0.0f, 1.0f);
}

bwColor::operator const float*() const
{
	return getColor();
}
