#include "bwStylePropertyType.h"

using namespace bWidgets;


bwStylePropertyTypeMap::MapType bwStylePropertyTypeMap::map{};

bwStylePropertyType& bwStylePropertyTypeMap::add(
        const std::string& identifier,
        bwStyleProperty::PropertyType prop_type)
{
	map.insert({identifier, {prop_type}});
	return lookup(identifier).value();
}

bwOptional<std::reference_wrapper<bwStylePropertyType>> bwStylePropertyTypeMap::lookup(const std::string& identifier)
{
	auto result = map.find(identifier);

	if (result != map.end()) {
		return result->second;
	}
	return {};
}
