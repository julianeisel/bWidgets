#include "Color.h"


using namespace bWidgets;

Color::Color(float red, float green, float blue, float alpha)
{
	setColor(red, green, blue, alpha);
}

void Color::setColor(float red, float green, float blue, float alpha)
{
	float rgba[4] = {red, green, blue, alpha};
	setColor(rgba);
}

void Color::setColor(const float _rgba[])
{
	rgba[0] = _rgba[0];
	rgba[1] = _rgba[1];
	rgba[2] = _rgba[2];
	rgba[3] = _rgba[3];
}

const float* Color::getColor() const
{
	return rgba;
}

float& Color::operator[](unsigned int index)
{
	return rgba[index];
}

Color::operator const float*() const
{
	return rgba;
}
