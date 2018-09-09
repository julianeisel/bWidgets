#include "css_background.h"
#include "css_border.h"
#include "css_color.h"
#include "css_custom_props.h"

#include "bwStyleStructTypeMap.h"

using namespace bWidgets;


bwStyleStructTypeMap::MapAlias bwStyleStructTypeMap::map{};

void bwStyleStructTypeMap::registerStyleStructTypes()
{
	bwStyleStructBackground::registerStructType();
	bwStyleStructBorder::registerStructType();
	bwStyleStructColor::registerStructType();
	bwStyleStructCustomProps::registerStructType();
}

bwStyleStructType& bwStyleStructTypeMap::add(const std::string& identifier)
{
	map.insert({identifier, {}});
	return map[identifier];
}

bwOptional<std::reference_wrapper<bwStyleStructType>> bwStyleStructTypeMap::lookup(const std::string& identifier)
{
	auto result = map.find(identifier);

	if (result != map.end()) {
		return result->second;
	}
	return {};
}
