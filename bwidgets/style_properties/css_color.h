#pragma once

#include "bwColor.h"
#include "bwStyleStruct.h"


namespace bWidgets {

class bwStyleStructColor : public bwStyleStruct
{
public:
	static void registerStructType();

	bwStyleStructColor();

	bwColor color;
};

} // namespace bWidgets
