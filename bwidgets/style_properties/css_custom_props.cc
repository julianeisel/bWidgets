#include "bwColor.h"
#include "bwStyleStructTypeMap.h"

#include "css_custom_props.h"

using namespace bWidgets;


void bwStyleStructCustomProps::registerStructType()
{
	bwStyleStructType& struct_type = bwStyleStructTypeMap::add("custom-properties");
	struct_type.addProperty("decoration-color", bwStyleProperty::TYPE_COLOR);
	struct_type.addProperty("shade-top", bwStyleProperty::TYPE_INTEGER);
	struct_type.addProperty("shade-bottom", bwStyleProperty::TYPE_INTEGER);
	struct_type.addProperty("draw-separator", bwStyleProperty::TYPE_BOOL);
}
