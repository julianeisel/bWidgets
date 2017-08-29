#include <math.h>

#include "bwColor.h"


using namespace bWidgets;

bwColor::bwColor(float red, float green, float blue, float alpha)
{
	setColor(red, green, blue, alpha);
}

bwColor::bwColor(float rgb, float alpha)
{
	setColor(rgb, alpha);
}

bwColor& bwColor::shade(const float rgb_shade, float alpha_shade)
{
	for (int i = 0; i < 3; i++) {
		rgba[i] = fmin(rgba[i] + rgb_shade, 1.0);
	}
	rgba[3] = fmin(rgba[3] + alpha_shade, 1.0);

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
	return rgba;
}

void bwColor::operator=(const float rgb)
{
	setColor(rgb);
}

float& bwColor::operator[](unsigned int index)
{
	return rgba[index];
}

bwColor::operator const float*() const
{
	return rgba;
}
