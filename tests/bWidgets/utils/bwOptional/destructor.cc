/*
 * Much of this is based on Clang's implementation of std::optional. For fair
 * credit and copyright compliance, the following notice additionally applies:
 *
 * Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT (in
 * the directory containing bwOptional.h)
 */

#include "gtest/gtest.h"

#include "bwOptional.h"

#include "TestUtilClasses.h"
#include "TestUtilMacros.h"

using namespace bWidgets;

template<typename _T> void test_type()
{
  static_assert(!std::is_trivially_destructible<_T>::value ||
                    std::is_trivially_destructible<bwOptional<_T>>::value,
                "");
}

TEST(bwOptional, destructor)
{
  static_assert(std::is_trivially_destructible<bwOptional<int>>::value, "");
  static_assert(std::is_trivially_destructible<bwOptional<TestUtilClasses::NoConstructors>>::value,
                "");
  static_assert(
      !std::is_trivially_destructible<bwOptional<TestUtilClasses::NonTrivialConstructors>>::value,
      "");
  test_type<int>();
  test_type<TestUtilClasses::NonTrivialConstructors>();
  test_type<TestUtilClasses::ExplicitNonTrivialConstructors>();

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(30);
    EXPECT_TRUE(lhs);
    EXPECT_EQ(lhs->tot_int_ctor_calls, 1);
    EXPECT_EQ(lhs->tot_instances, 1);
    lhs.~bwOptional();
    EXPECT_TRUE(lhs);
    EXPECT_EQ(lhs->tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(30);
    EXPECT_TRUE(lhs);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
  }
  EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
}
