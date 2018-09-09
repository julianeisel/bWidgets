#pragma once

#include "bwColor.h"
#include "bwStyleStruct.h"


namespace bWidgets {

class bwStyleStructBackground : public bwStyleStruct
{
public:
	static void registerStructType();

	bwStyleStructBackground() = default;

private:
	bwColor background_color;
};

} // namespace bWidgets