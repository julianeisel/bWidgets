#pragma once

#include <unordered_map>

#include "bwStyleStruct.h"
#include "bwUtil.h"


namespace bWidgets {

class bwStyleStructTypeMap
{
public:
	static void registerStyleStructTypes();

	static bwStyleStructType& add(const std::string&);
	static bwOptional<std::reference_wrapper<bwStyleStructType>> lookup(const std::string&);

private:
	using MapAlias = std::unordered_map<std::string, bwStyleStructType>;
	static MapAlias map;
};

} // namespace bWidgets
