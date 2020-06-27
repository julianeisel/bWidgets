#pragma once

#include <optional>
#include <string>
#include <vector>
#include <memory>

namespace bWidgets {

class bwColor;

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
  void setValue(const bwColor&);
  void setValue(const bwStyleProperty&);
  void setValueToDefault();

  void setDefaultValue(bool);
  void setDefaultValue(int);
  void setDefaultValue(float);
  void setDefaultValue(const bwColor&);

  auto getIdentifier() const -> const std::string&;
  auto getType() const -> Type;

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
  using PropertyList = std::vector<std::unique_ptr<bwStyleProperty>>;
  using iterator = PropertyList::iterator;
  using const_iterator = PropertyList::const_iterator;

  auto addBool(const std::string& name, bool& reference) -> bwStyleProperty&;
  auto addBool(const std::string& name) -> bwStyleProperty&;
  auto addInteger(const std::string& name, int& reference) -> bwStyleProperty&;
  auto addInteger(const std::string& name) -> bwStyleProperty&;
  auto addFloat(const std::string& name, float& reference) -> bwStyleProperty&;
  auto addFloat(const std::string& name) -> bwStyleProperty&;
  auto addColor(const std::string& name, class bwColor& reference) -> bwStyleProperty&;
  auto addColor(const std::string& name) -> bwStyleProperty&;
  auto addProperty(const std::string& name, const bwStyleProperty::Type prop_type)
      -> bwStyleProperty&;

  auto lookup(const std::string& name) const -> const bwStyleProperty*;

  auto begin() -> iterator;
  auto end() -> iterator;
  auto begin() const -> const_iterator;
  auto end() const -> const_iterator;

 private:
  PropertyList properties{};
};

}  // namespace bWidgets
