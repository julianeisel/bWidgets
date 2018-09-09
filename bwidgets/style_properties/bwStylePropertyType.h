#pragma once

#include <unordered_map>

#include "bwStyleProperties.h"


namespace bWidgets {

class bwStylePropertyType
{
	friend class bwStylePropertyTypeMap;

public:
	bwStylePropertyType(bwStyleProperty::PropertyType _type) :
	    type(_type)
	{}

private:
	bwStyleProperty::PropertyType type;
};

class bwStylePropertyTypeMap
{
public:
	static bwStylePropertyType& add(
	        const std::string&,
	        bwStyleProperty::PropertyType);

	static bwOptional<std::reference_wrapper<bwStylePropertyType>> lookup(const std::string&);

private:
	using MapType = std::unordered_map<std::string, bwStylePropertyType>;
	static MapType map;
};

} // namespace bWidgets
