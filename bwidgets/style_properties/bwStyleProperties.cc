#include <cassert>
#include <iostream>

#include "bwColor.h"

#include "bwStyleProperties.h"

using namespace bWidgets;


namespace bWidgets {

template <typename _Type>
class bwStylePropertyInternal : public bwStyleProperty
{
public:
	bwStylePropertyInternal(const std::string& name, _Type& reference);
	bwStylePropertyInternal(const std::string& name);

	void setValue(const _Type& value);
	const _Type getValue() const;

	_Type default_value{};

private:
	_Type& reference;
	// If #bwStylePropertyInternal constructor is called without reference
	// parameter, this value is used and reference references this.
	_Type value;
};

} // namespace bWidgets


// --------------------------------------------------------------------

static bool property_value_is_copyable(
        const bwStyleProperty& destination,
        const bwStyleProperty& source)
{
	bwStyleProperty::PropertyType dest_type = destination.getType();
	bwStyleProperty::PropertyType src_type = source.getType();

	switch (dest_type) {
		case bwStyleProperty::TYPE_BOOL:
			// Could also allow int/float here and check for != 0.
			return (src_type == bwStyleProperty::TYPE_BOOL);
		case bwStyleProperty::TYPE_INTEGER:
			// Could also allow bool here.
			return (src_type == bwStyleProperty::TYPE_INTEGER);
		case bwStyleProperty::TYPE_FLOAT:
			// Could also allow bool here.
			return ((src_type == bwStyleProperty::TYPE_INTEGER) ||
			        (src_type == bwStyleProperty::TYPE_FLOAT));
		case bwStyleProperty::TYPE_COLOR:
			return (src_type == bwStyleProperty::TYPE_COLOR);
		default:
			assert(0);
			return false;
	}
}

template<typename _Type>
void bwStylePropertyInternal<_Type>::setValue(const _Type& value)
{
	reference = value;
}
template<typename _Type>
const _Type bwStylePropertyInternal<_Type>::getValue() const
{
	return reference;
}

void bwStyleProperty::setValue(bool value)
{
	bwStylePropertyInternal<bool>& property = static_cast<bwStylePropertyInternal<bool>&>(*this);
	assert(type == TYPE_BOOL);
	property.setValue(value);
}
void bwStyleProperty::setValue(int value)
{
	bwStylePropertyInternal<int>& property = static_cast<bwStylePropertyInternal<int>&>(*this);
	assert(type == TYPE_INTEGER);
	property.setValue(value);
}
void bwStyleProperty::setValue(float value)
{
	bwStylePropertyInternal<float>& property = static_cast<bwStylePropertyInternal<float>&>(*this);
	assert(type == TYPE_FLOAT);
	property.setValue(value);
}
void bwStyleProperty::setValue(const bwColor& value)
{
	bwStylePropertyInternal<bwColor>& property = static_cast<bwStylePropertyInternal<bwColor>&>(*this);
	assert(type == TYPE_COLOR);
	property.setValue(value);
}

template<typename _Type>
static void property_copy_value(
        bwStyleProperty& destination_property_base,
        const bwStyleProperty& source_property_base)
{
	if (property_value_is_copyable(destination_property_base, source_property_base)) {
		const auto& from_property = static_cast<const bwStylePropertyInternal<_Type>&>(source_property_base);
		destination_property_base.setValue(from_property.getValue());
	}
	else {
		throw "Invalid style-property value.";
	}
}

void bwStyleProperty::setValue(const bwStyleProperty& from_property_base)
{
	switch (type) {
		case bwStyleProperty::TYPE_BOOL:
			property_copy_value<bool>(*this, from_property_base);
			break;
		case bwStyleProperty::TYPE_FLOAT:
			property_copy_value<float>(*this, from_property_base);
			break;
		case bwStyleProperty::TYPE_COLOR:
			property_copy_value<bwColor>(*this, from_property_base);
			break;
		case bwStyleProperty::TYPE_INTEGER:
		default:
			property_copy_value<int>(*this, from_property_base);
			break;
	}
}

void bwStyleProperty::setValueToDefault()
{
	switch (type) {
		case bwStyleProperty::TYPE_BOOL:
		{
			bwStylePropertyInternal<bool>& property = static_cast<bwStylePropertyInternal<bool>&>(*this);
			property.setValue(property.default_value);
			break;
		}
		case bwStyleProperty::TYPE_INTEGER:
		{
			bwStylePropertyInternal<int>& property = static_cast<bwStylePropertyInternal<int>&>(*this);
			property.setValue(property.default_value);
			break;
		}
		case bwStyleProperty::TYPE_FLOAT:
		{
			bwStylePropertyInternal<float>& property = static_cast<bwStylePropertyInternal<float>&>(*this);
			property.setValue(property.default_value);
			break;
		}
		case bwStyleProperty::TYPE_COLOR:
		{
			bwStylePropertyInternal<bwColor>& property = static_cast<bwStylePropertyInternal<bwColor>&>(*this);
			property.setValue(property.default_value);
			break;
		}
		default:
			assert(0);
			break;
	}
}

void bwStyleProperty::setDefaultValue(bool value)
{
	bwStylePropertyInternal<bool>& property = static_cast<bwStylePropertyInternal<bool>&>(*this);
	assert(type == TYPE_BOOL);
	property.default_value = value;
}
void bwStyleProperty::setDefaultValue(int value)
{
	bwStylePropertyInternal<int>& property = static_cast<bwStylePropertyInternal<int>&>(*this);
	assert(type == TYPE_INTEGER);
	property.default_value = value;
}
void bwStyleProperty::setDefaultValue(float value)
{
	bwStylePropertyInternal<float>& property = static_cast<bwStylePropertyInternal<float>&>(*this);
	assert(type == TYPE_FLOAT);
	property.default_value = value;
}
void bwStyleProperty::setDefaultValue(const bwColor& value)
{
	bwStylePropertyInternal<bwColor>& property = static_cast<bwStylePropertyInternal<bwColor>&>(*this);
	assert(type == TYPE_COLOR);
	property.default_value = value;
}

// --------------------------------------------------------------------
/**
 * Simple type-trait mechanism to get the right bwStyleProperty::PropType
 * from the data-type of the property (bool, int, bwColor, ...) and vise versa.
 *
 * \{
 */

namespace bWidgets {

template<typename> struct PropType;

template<> struct PropType<bool>
{
	const static bwStyleProperty::PropertyType type{bwStyleProperty::TYPE_BOOL};
};
template<> struct PropType<int>
{
	const static bwStyleProperty::PropertyType type{bwStyleProperty::TYPE_INTEGER};
};
template<> struct PropType<float>
{
	const static bwStyleProperty::PropertyType type{bwStyleProperty::TYPE_FLOAT};
};
template<> struct PropType<bwColor>
{
	const static bwStyleProperty::PropertyType type{bwStyleProperty::TYPE_COLOR};
};


#if 0
template<enum bwStyleProperty::PropertyType> struct PropDataType;

template<> struct PropDataType<bwStyleProperty::TYPE_BOOL>
{
	using type = bool;
};
template<> struct PropDataType<bwStyleProperty::TYPE_INTEGER>
{
	using type = int;
};
template<> struct PropDataType<bwStyleProperty::TYPE_FLOAT>
{
	using type = float;
};
template<> struct PropDataType<bwStyleProperty::TYPE_COLOR>
{
	using type = bwColor;
};
#endif

} // namespace bWidgets

/** \} */


// --------------------------------------------------------------------
/**
 * \name Property registration
 * \{
 */

bwStyleProperty::bwStyleProperty(
        const std::string& identifier,
        enum PropertyType type) :
    identifier(identifier), type(type)
{
	
}

template<typename _Type>
bwStylePropertyInternal<_Type>::bwStylePropertyInternal(
        const std::string& name,
        _Type& reference) :
    bwStyleProperty(name, PropType<_Type>::type),
    reference(reference)
{
	
}
template<typename _Type>
bwStylePropertyInternal<_Type>::bwStylePropertyInternal(
        const std::string& name) :
    bwStyleProperty(name, PropType<_Type>::type),
    reference(value)
{
	
}

template<typename _Type>
static bwStyleProperty& properties_add_property(
        bwStyleProperties::PropertyList& properties,
        const std::string& name,
        _Type& reference)
{
	properties.push_back(bwPointer_new<bwStylePropertyInternal<_Type>>(name, reference));
	return *properties.back();
}
template<typename _Type>
static bwStyleProperty& properties_add_property(
        bwStyleProperties::PropertyList& properties,
        const std::string& name)
{
	properties.push_back(bwPointer_new<bwStylePropertyInternal<_Type>>(name));
	return *properties.back();
}

bwStyleProperty& bwStyleProperties::addBool(const std::string& name, bool& reference)
{
	return properties_add_property<bool>(properties, name, reference);
}
bwStyleProperty& bwStyleProperties::addBool(const std::string& name)
{
	return properties_add_property<bool>(properties, name);
}
bwStyleProperty& bwStyleProperties::addInteger(const std::string& name, int& reference)
{
	return properties_add_property<int>(properties, name, reference);
}
bwStyleProperty& bwStyleProperties::addInteger(const std::string& name)
{
	return properties_add_property<int>(properties, name);
}
bwStyleProperty& bwStyleProperties::addFloat(const std::string& name, float& reference)
{
	return properties_add_property<float>(properties, name, reference);
}
bwStyleProperty& bwStyleProperties::addFloat(const std::string& name)
{
	return properties_add_property<float>(properties, name);
}
bwStyleProperty& bwStyleProperties::addColor(const std::string& name, bwColor& reference)
{
	return properties_add_property<bwColor>(properties, name, reference);
}
bwStyleProperty& bwStyleProperties::addColor(const std::string& name)
{
	return properties_add_property<bwColor>(properties, name);
}

bwStyleProperty& bwStyleProperties::addProperty(
        const std::string& name,
        const bwStyleProperty::PropertyType prop_type)
{
//	properties_add_property<PropDataType<prop_type>::type>(properties, name);
//	properties.push_back(bwPointer_new<bwStylePropertyInternal<prop_type>(name));
	switch (prop_type) {
		case bwStyleProperty::TYPE_BOOL:
			return properties_add_property<bool>(properties, name);
		case bwStyleProperty::TYPE_INTEGER:
			return properties_add_property<int>(properties, name);
		case bwStyleProperty::TYPE_FLOAT:
			return properties_add_property<float>(properties, name);
		case bwStyleProperty::TYPE_COLOR:
			return properties_add_property<bwColor>(properties, name);
		default:
			assert(0);
			return properties_add_property<int>(properties, name);
	}
}

/** \} */


// --------------------------------------------------------------------

const std::string& bwStyleProperty::getIdentifier() const
{
	return identifier;
}
bwStyleProperty::PropertyType bwStyleProperty::getType() const
{
	return type;
}

// --------------------------------------------------------------------

bwOptional<std::reference_wrapper<const bwStyleProperty>> bwStyleProperties::lookup(const std::string& name) const
{
	for (const auto& property : properties) {
		if (property->getIdentifier() == name) {
			return *property;
		}
	}

	return nullopt;
}

bwStyleProperties::iterator bwStyleProperties::begin()
{
	return properties.begin();
}
bwStyleProperties::iterator bwStyleProperties::end()
{
	return properties.end();
}
bwStyleProperties::const_iterator bwStyleProperties::begin() const
{
	return properties.begin();
}
bwStyleProperties::const_iterator bwStyleProperties::end() const
{
	return properties.end();
}
