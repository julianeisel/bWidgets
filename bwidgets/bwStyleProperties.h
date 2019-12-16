#pragma once

#include <string>
#include <vector>

#include "bwUtil.h"

namespace bWidgets {

/**
 * \class bwStyleProperty
 * \brief Simple class for managing properties that can be manipulated through
 *        stylesheets (CSS).
 *
 * Idea of the bwStyleProperty is to add a string identifier to a variable
 * (basic RTTI) so that stylesheet rules can be mapped to this concrete
 * variable. Use the bwStyleProperties class to manage (add, lookup and
 * iterate) a list of properties.
 *
 * It is possible to reference an existing variable with a bwStyleProperty,
 * meaning the property is __not__ owner of it. To do so call the
 * bwStyleProperties::addFoo() function with the reference argument set. Such
 * usage looks like this:
 * \code
 * bwStyleProperties properties;
 * int some_int = 0;
 *
 * bwStyleProperty& property = properties.addInteger("int_name", some_int);
 * property.setValue(42);
 * assert(some_int == 42);
 * \endcode
 */
class bwStyleProperty {
  template<typename> friend class bwStylePropertyInternal;

 public:
  enum class Type {
    BOOL,
    INTEGER,
    FLOAT,
    COLOR,
  };

  void setValue(bool);
  void setValue(int);
  void setValue(float);
  void setValue(const class bwColor&);
  void setValue(const bwStyleProperty&);
  void setValueToDefault();

  void setDefaultValue(bool);
  void setDefaultValue(int);
  void setDefaultValue(float);
  void setDefaultValue(const class bwColor&);

  const std::string& getIdentifier() const;
  Type getType() const;

 private:
  bwStyleProperty(std::string identifier, enum Type type);

  const std::string identifier;
  enum Type type;
};

/**
 * \class bwStyleProperties
 * \brief Manage a list of properties (bwStyleProperty instances).
 *
 * This class can be used to do the following:
 * * Add/register new properties (addFoo() functions).
 * * Lookup a property from its identifier (lookup() function).
 * * Get iterators to iterate over all properties.
 */
class bwStyleProperties {
 public:
  // Store properties as pointer, they are actually created as bwStylePropertyInternal instances.
  using PropertyList = std::vector<bwPtr<bwStyleProperty>>;

  bwStyleProperty& addBool(const std::string& name, bool& reference);
  bwStyleProperty& addBool(const std::string& name);
  bwStyleProperty& addInteger(const std::string& name, int& reference);
  bwStyleProperty& addInteger(const std::string& name);
  bwStyleProperty& addFloat(const std::string& name, float& reference);
  bwStyleProperty& addFloat(const std::string& name);
  bwStyleProperty& addColor(const std::string& name, class bwColor& reference);
  bwStyleProperty& addColor(const std::string& name);
  bwStyleProperty& addProperty(const std::string& name,
                               const bwStyleProperty::Type prop_type);

  bwOptional<std::reference_wrapper<const bwStyleProperty>> lookup(const std::string& name) const;

  using iterator = PropertyList::iterator;
  iterator begin();
  iterator end();
  using const_iterator = PropertyList::const_iterator;
  const_iterator begin() const;
  const_iterator end() const;

 private:
  PropertyList properties{};
};

}  // namespace bWidgets
