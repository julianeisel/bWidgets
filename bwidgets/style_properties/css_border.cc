#include "bwColor.h"
#include "bwStyleStructTypeMap.h"

#include "css_border.h"

using namespace bWidgets;


void bwStyleStructBorder::registerStructType()
{
	bwStyleStructType& struct_type = bwStyleStructTypeMap::add("border");
	struct_type.addProperty("border-color", bwStyleProperty::TYPE_COLOR);
	struct_type.addProperty("border-radius", bwStyleProperty::TYPE_FLOAT);
}
