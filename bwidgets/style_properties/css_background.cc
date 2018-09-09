#include "bwStyleStructTypeMap.h"

#include "css_background.h"

using namespace bWidgets;


void bwStyleStructBackground::registerStructType()
{
	bwStyleStructType& struct_type = bwStyleStructTypeMap::add("background");
	struct_type.addProperty("background-color", bwStyleProperty::TYPE_COLOR);
}
