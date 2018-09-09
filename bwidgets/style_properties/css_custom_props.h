#pragma once

#include "bwColor.h"
#include "bwStyleStruct.h"


namespace bWidgets {

class bwStyleStructCustomProps : public bwStyleStruct
{
public:
	static void registerStructType();

private:
	bwColor color;
	int shade_top, shade_bottom;
};

} // namespace bWidgets
