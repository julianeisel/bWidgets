#include "gtest/gtest.h"

#include "bwColor.h"
#include "bwStyleProperties.h"

using namespace bWidgets;

// --------------------------------------------------------------------
// bwStyleProperties

TEST(bwStyleProperties, lookup_empty)
{
  bwStyleProperties properties;
  EXPECT_FALSE(properties.lookup("null"));
}

TEST(bwStyleProperties, addBool)
{
  bwStyleProperties properties;

  properties.addBool("test_bool");
  EXPECT_TRUE(properties.lookup("test_bool"));
}
TEST(bwStyleProperties, addBool_reference)
{
  bwStyleProperties properties;
  bool test_bool = true;

  properties.addBool("test_bool", test_bool);
  EXPECT_TRUE(properties.lookup("test_bool"));
  EXPECT_TRUE(test_bool);
}

TEST(bwStyleProperties, addInteger)
{
  bwStyleProperties properties;

  properties.addInteger("test_int");
  EXPECT_TRUE(properties.lookup("test_int"));
}
TEST(bwStyleProperties, addInteger_reference)
{
  bwStyleProperties properties;
  int test_int = 42;

  properties.addInteger("test_int", test_int);
  EXPECT_TRUE(properties.lookup("test_int"));
  EXPECT_EQ(test_int, 42);
}

TEST(bwStyleProperties, addFloat)
{
  bwStyleProperties properties;

  properties.addFloat("test_float");
  EXPECT_TRUE(properties.lookup("test_float"));
}
TEST(bwStyleProperties, addFloat_reference)
{
  bwStyleProperties properties;
  float test_float = 42.0f;

  properties.addFloat("test_float", test_float);
  EXPECT_TRUE(properties.lookup("test_float"));
  EXPECT_EQ(test_float, 42.0f);
}

TEST(bwStyleProperties, addColor)
{
  bwStyleProperties properties;

  properties.addColor("test_color");
  EXPECT_TRUE(properties.lookup("test_color"));
}
TEST(bwStyleProperties, addColor_reference)
{
  bwStyleProperties properties;
  bwColor test_color{0.5f};

  properties.addColor("test_color", test_color);
  EXPECT_TRUE(properties.lookup("test_color"));
  EXPECT_EQ(test_color, bwColor(0.5f));
}

TEST(bwStyleProperties, addProperty_bool)
{
  bwStyleProperties properties;

  properties.addProperty("test_bool", bwStyleProperty::TYPE_BOOL);
  EXPECT_TRUE(properties.lookup("test_bool"));
}
TEST(bwStyleProperties, addProperty_integer)
{
  bwStyleProperties properties;

  properties.addProperty("test_integer", bwStyleProperty::TYPE_INTEGER);
  EXPECT_TRUE(properties.lookup("test_integer"));
}
TEST(bwStyleProperties, addProperty_float)
{
  bwStyleProperties properties;

  properties.addProperty("test_float", bwStyleProperty::TYPE_FLOAT);
  EXPECT_TRUE(properties.lookup("test_float"));
}
TEST(bwStyleProperties, addProperty_color)
{
  bwStyleProperties properties;

  properties.addProperty("test_color", bwStyleProperty::TYPE_COLOR);
  EXPECT_TRUE(properties.lookup("test_color"));
}

TEST(bwStyleProperties, begin_end_empty)
{
  bwStyleProperties properties;
  EXPECT_EQ(properties.begin(), properties.end());
}

TEST(bwStyleProperties, iterate_empty)
{
  bwStyleProperties properties;

  for (const auto &property : properties) {
    GTEST_FATAL_FAILURE_("Unexpected item in bwStyleProperties iterator.");
    (void)property;
  }
}
TEST(bwStyleProperties, iterate)
{
  bwStyleProperties properties;

  properties.addBool("test_bool");
  properties.addInteger("test_integer");
  properties.addFloat("test_float");
  properties.addColor("test_color");

  for (const auto &property : properties) {
    const bwStyleProperty::PropertyType property_type = property->getType();
    const std::string &identifier = property->getIdentifier();

    switch (property_type) {
      case bwStyleProperty::TYPE_BOOL:
        EXPECT_EQ(identifier, "test_bool");
        break;
      case bwStyleProperty::TYPE_INTEGER:
        EXPECT_EQ(identifier, "test_integer");
        break;
      case bwStyleProperty::TYPE_FLOAT:
        EXPECT_EQ(identifier, "test_float");
        break;
      case bwStyleProperty::TYPE_COLOR:
        EXPECT_EQ(identifier, "test_color");
        break;
      default:
        GTEST_FATAL_FAILURE_("Unexpected item in bwStyleProperties iterator.");
        break;
    }
  }
}

// --------------------------------------------------------------------
// bwStyleProperty

TEST(bwStyleProperty, setValue_bool)
{
  bwStyleProperties properties;
  bool test_bool = true;

  bwStyleProperty &property = properties.addBool("test_bool", test_bool);

  property.setValue(false);
  EXPECT_FALSE(test_bool);
  property.setValue(true);
  EXPECT_TRUE(test_bool);
  // Test same value again, to ensure it doesn't just toggle.
  property.setValue(true);
  EXPECT_TRUE(test_bool);
}
TEST(bwStyleProperty, setValue_integer)
{
  bwStyleProperties properties;
  int test_integer = 1;

  bwStyleProperty &property = properties.addInteger("test_integer", test_integer);

  property.setValue(10);
  EXPECT_EQ(test_integer, 10);
  property.setValue(0);
  EXPECT_EQ(test_integer, 0);
  property.setValue(-42);
  EXPECT_EQ(test_integer, -42);
  property.setValue(99);
  EXPECT_EQ(test_integer, 99);
}
TEST(bwStyleProperty, setValue_float)
{
  bwStyleProperties properties;
  float test_float = true;

  bwStyleProperty &property = properties.addFloat("test_float", test_float);

  // Conversion from int to float based on property type not possible yet, so
  // have to explicitly pass as float.
  property.setValue(10.0f);
  EXPECT_EQ(test_float, 10.0f);
  property.setValue(0.0f);
  EXPECT_EQ(test_float, 0.0f);
  property.setValue(-42.0f);
  EXPECT_EQ(test_float, -42.0f);
  property.setValue(99.0f);
  EXPECT_EQ(test_float, 99.0f);
}
TEST(bwStyleProperty, setValue_color)
{
  bwStyleProperties properties;
  bwColor test_color;

  bwStyleProperty &property = properties.addColor("test_color", test_color);

  property.setValue(bwColor(1.0f));
  EXPECT_EQ(test_color, bwColor(255u, 255u, 255u, 255u));
  property.setValue(bwColor(0u));
  EXPECT_EQ(test_color, bwColor(0u, 0u, 0u));
  property.setValue(bwColor(42u));
  EXPECT_EQ(test_color, bwColor(42u));
  property.setValue(bwColor(102u));
  EXPECT_EQ(test_color, bwColor(0.4f));
  // TODO What about color values outside of range? Should probably test that in bwColor tests.
}
TEST(bwStyleProperty, setValue_from_other_property)
{
  bwStyleProperties properties;
  int int_copy_to = 0;
  int int_copy_from = 42;

  bwStyleProperty &property_copy_to = properties.addInteger("test_copy_to", int_copy_to);
  bwStyleProperty &property_copy_from = properties.addInteger("test_copy_from", int_copy_from);

  property_copy_to.setValue(property_copy_from);
  EXPECT_EQ(int_copy_to, int_copy_from);
  // Ensure setting value of _to doesn't affect _from.
  property_copy_to.setValue(50);
  EXPECT_EQ(int_copy_to, 50);
  EXPECT_EQ(int_copy_from, 42);
}
TEST(bwStyleProperty, setValue_from_self)
{
  bwStyleProperties properties;
  int test_integer = 42;

  bwStyleProperty &property = properties.addInteger("test_integer", test_integer);

  property.setValue(property);
  EXPECT_EQ(test_integer, 42);
  property.setValue(4);
  EXPECT_EQ(test_integer, 4);
}

TEST(bwStyleProperty, setDefaultValue_bool_reference)
{
  bwStyleProperties properties;
  bool test_bool = true;

  bwStyleProperty &property = properties.addBool("test_bool", test_bool);

  property.setDefaultValue(false);

  EXPECT_TRUE(test_bool);
  property.setValueToDefault();
  EXPECT_FALSE(test_bool);
}
TEST(bwStyleProperty, setDefaultValue_integer_reference)
{
  bwStyleProperties properties;
  int test_integer = 42;

  bwStyleProperty &property = properties.addInteger("test_integer", test_integer);

  property.setDefaultValue(123);

  EXPECT_EQ(test_integer, 42);
  property.setValueToDefault();
  EXPECT_EQ(test_integer, 123);
}
TEST(bwStyleProperty, setDefaultValue_float_reference)
{
  bwStyleProperties properties;
  float test_float = 42.0f;

  bwStyleProperty &property = properties.addFloat("test_float", test_float);

  property.setDefaultValue(123.0f);

  EXPECT_EQ(test_float, 42.0f);
  property.setValueToDefault();
  EXPECT_EQ(test_float, 123.0f);
}
TEST(bwStyleProperty, setDefaultValue_color_reference)
{
  bwStyleProperties properties;
  bwColor test_color{0.8f};

  bwStyleProperty &property = properties.addColor("test_color", test_color);

  property.setDefaultValue(bwColor{0.3f});

  EXPECT_EQ(test_color, 0.8f);
  property.setValueToDefault();
  EXPECT_EQ(test_color, 0.3f);
}
