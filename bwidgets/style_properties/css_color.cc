#include "bwColor.h"
#include "bwStyleStructTypeMap.h"

#include "css_color.h"

using namespace bWidgets;


void bwStyleStructColor::registerStructType()
{
	bwStyleStructType& struct_type = bwStyleStructTypeMap::add("color");
	struct_type.addProperty("color", bwStyleProperty::TYPE_COLOR);
}

bwStyleStructColor::bwStyleStructColor() :
        bwStyleStruct(INHERITS_TRUE)
{

}
