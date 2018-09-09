#include "bwStyleStruct.h"

using namespace bWidgets;


bwStylePropertyType& bwStyleStructType::addProperty(
        const std::string& identifier,
        bwStyleProperty::PropertyType prop_type)
{
	return bwStylePropertyTypeMap::add(identifier, prop_type);
}
