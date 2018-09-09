#pragma once

#include "bwColor.h"
#include "bwStyleStruct.h"


namespace bWidgets {

class bwStyleStructBorder : public bwStyleStruct
{
public:
	static void registerStructType();

private:
	bwColor border_color;
	float border_radius;
};

} // namespace bWidgets
