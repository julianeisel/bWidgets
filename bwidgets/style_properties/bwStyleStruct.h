#pragma once

#include <string>

#include "bwStylePropertyType.h"


namespace bWidgets {

class bwStyleStruct
{
public:
	enum Inherits {
		INHERITS_TRUE,
		INHERITS_FALSE
	};

	bwStyleStruct() = default;
	bwStyleStruct(Inherits _inherit) : inherit(_inherit) {}

protected:
	Inherits inherit{INHERITS_FALSE};
};

class bwStyleStructType
{
public:
	bwStylePropertyType& addProperty(
	        const std::string&,
	        bwStyleProperty::PropertyType);
};

} // namespace bWidgets
