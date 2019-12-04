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

TEST(bwOptional, operator_assign_nullopt)
{
  {
    bwOptional<int> opt;
    opt = nullopt;
    EXPECT_FALSE(opt);
  }
  {
    bwOptional<int> opt(42);
    opt = nullopt;
    EXPECT_FALSE(opt);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;

    opt = nullopt;
    EXPECT_FALSE(opt);
    TestUtilClasses::NonTrivialConstructors::assert_no_instance();
  }
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt{
        TestUtilClasses::NonTrivialConstructors(42)};

    EXPECT_TRUE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    opt = nullopt;
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
}

TEST(bwOptional, operator_assign_copy)
{
  {
    bwOptional<int> opt;
    bwOptional<int> opt_other;

    opt = opt_other;
    EXPECT_FALSE(opt);
  }
  {
    bwOptional<int> opt;
    bwOptional<int> opt_other(42);

    opt = opt_other;
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, 42);
  }
  {
    bwOptional<int> opt(42);
    bwOptional<int> opt_other;

    opt = opt_other;
    EXPECT_FALSE(opt);
  }
  {
    bwOptional<int> opt;
    bwOptional<int> opt_other(42);

    opt = opt_other;
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, 42);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other;

    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    opt = opt_other;
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other;

    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    opt = opt_other;
    EXPECT_FALSE(opt);
    /* Previous value of opt_other should be destructed now. */
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other(50);

    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    opt = opt_other;
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 50);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 2);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other(11);

    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 42);
    opt = opt_other;
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 11);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 2);
  }
  {
    bwOptional<int> opt(42);
    bwOptional<int> opt_other;

    // Check return value.
    opt_other = opt = opt_other;
    EXPECT_FALSE(opt);
    EXPECT_FALSE(opt_other);
  }
  static_assert(!std::is_copy_assignable<bwOptional<TestUtilClasses::NoCopyConstructor>>::value,
                "");
  static_assert(
      !std::is_copy_assignable<bwOptional<TestUtilClasses::NoCopyAssignmentOperator>>::value, "");
}

TEST(bwOptional, operator_assign_move)
{
  {
    bwOptional<int> opt;
    bwOptional<int> opt_other;

    opt = std::move(opt_other);
    EXPECT_FALSE(opt);
  }
  {
    bwOptional<int> opt;
    bwOptional<int> opt_other{42};

    opt = std::move(opt_other);
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, 42);
  }
  {
    bwOptional<int> opt{42};
    bwOptional<int> opt_other;

    opt = std::move(opt_other);
    EXPECT_FALSE(opt);
  }
  {
    bwOptional<int> opt{7};
    bwOptional<int> opt_other{42};

    opt = std::move(opt_other);
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, 42);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(54);

    opt = {};
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other;

    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    opt = std::move(opt_other);
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other;

    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    opt = std::move(opt_other);
    EXPECT_FALSE(opt);
    /* Previous value of opt_other should be destructed now. */
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other(42);

    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    opt = std::move(opt_other);
    EXPECT_TRUE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 2);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt{42};
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other{11};

    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 42);
    opt = std::move(opt_other);
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 11);
    // No destructor should be called when assigning.
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 2);
  }
  {
    bwOptional<int> opt{42};
    bwOptional<int> opt_other;

    // Check return value.
    opt_other = std::move(opt) = std::move(opt_other);
    EXPECT_FALSE(opt);
    EXPECT_FALSE(opt_other);
  }
  static_assert(!std::is_move_assignable<bwOptional<TestUtilClasses::NoMoveConstructor>>::value,
                "");
  static_assert(
      !std::is_move_assignable<bwOptional<TestUtilClasses::NoMoveAssignmentOperator>>::value, "");
}

TEST(bwOptional, operator_assign_value)
{
  {
    bwOptional<int> opt;

    opt = 13;
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, 13);
  }
  {
    bwOptional<int> opt;
    opt = 'f';
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, static_cast<int>('f'));
  }
  {
    int value = 3;
    bwOptional<void*> opt;
    opt = &value;
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, &value);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;

    opt = 15;
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 15);
    EXPECT_EQ(opt->tot_int_ctor_calls, 1);
    EXPECT_EQ(opt->tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(20);

    opt = 15;
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->_value, 15);
    EXPECT_EQ(opt->tot_int_ctor_calls, 2);
    EXPECT_EQ(opt->tot_instances, 1);
  }
  {
    bwOptional<std::vector<int>> opt;

    opt = {40, 21, 42};
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->size(), 3);
  }
  {
    bwOptional<std::vector<int>> opt(in_place, {5, 4});

    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->size(), 2);
    opt = {40, 21, 42};
    EXPECT_TRUE(opt);
    EXPECT_EQ(opt->size(), 3);
  }
}

TEST(bwOptional, operator_assign_convert_copy)
{
  {
    bwOptional<short> rhs;
    bwOptional<int> lhs;

    lhs = rhs;
    EXPECT_FALSE(lhs);
  }
  {
    bwOptional<short> rhs(42);
    bwOptional<int> lhs;

    lhs = rhs;
    EXPECT_TRUE(lhs);
    EXPECT_EQ(*lhs, 42);
  }
  {
    bwOptional<short> rhs(42);
    bwOptional<int> lhs(2);

    lhs = rhs;
    EXPECT_TRUE(lhs);
    EXPECT_EQ(*lhs, 42);
  }
  {
    bwOptional<short> rhs;
    bwOptional<int> lhs(50);

    lhs = rhs;
    EXPECT_FALSE(lhs);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs;
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs;

    lhs = rhs;
    TestUtilClasses::NonTrivialConstructors::assert_no_instance();
    EXPECT_FALSE(lhs);
  }
  {
    bwOptional<int> rhs(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs;

    lhs = rhs;
    EXPECT_TRUE(lhs);
    EXPECT_EQ(lhs->_value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(in_place);

    lhs = rhs;
    EXPECT_TRUE(lhs);
    EXPECT_EQ(lhs->_value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs;
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(30);

    lhs = rhs;
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    EXPECT_FALSE(lhs);
  }
}

TEST(bwOptional, operator_assign_convert_move)
{
  {
    bwOptional<short> rhs;
    bwOptional<int> lhs;

    lhs = std::move(rhs);
    EXPECT_FALSE(lhs);
  }
  {
    bwOptional<short> rhs(42);
    bwOptional<int> lhs;

    lhs = std::move(rhs);
    EXPECT_TRUE(lhs);
    EXPECT_EQ(*lhs, 42);
  }
  {
    bwOptional<short> rhs(42);
    bwOptional<int> lhs(2);

    lhs = std::move(rhs);
    EXPECT_TRUE(lhs);
    EXPECT_EQ(*lhs, 42);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs;
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs;

    lhs = std::move(rhs);
    TestUtilClasses::NonTrivialConstructors::assert_no_instance();
    EXPECT_FALSE(lhs);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs;

    lhs = std::move(rhs);
    EXPECT_TRUE(lhs);
    EXPECT_EQ(lhs->_value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs(42);
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(in_place);

    lhs = std::move(rhs);
    EXPECT_TRUE(lhs);
    EXPECT_EQ(lhs->_value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<int> rhs;
    bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(30);

    lhs = std::move(rhs);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    EXPECT_FALSE(lhs);
  }
}

/**
 * Causes a linking error when trying to use address-of operator (&).
 */
struct UndefinedAddressOfOperator {
  constexpr UndefinedAddressOfOperator* operator&() const;
  constexpr int foo() const
  {
    return 18;
  }
};

TEST(bwOptional, operator_arrow_non_const)
{
  {
    bwOptional<TestUtilClasses::SimpleFunction> opt(in_place);
    EXPECT_EQ(opt->foo(), 23);
  }
  {
    bwOptional<TestUtilClasses::SimpleFunction> opt;
    ::testing::StaticAssertTypeEq<decltype(opt.operator->()), TestUtilClasses::SimpleFunction*>();
  }
  {
    bwOptional<UndefinedAddressOfOperator> opt(in_place);
    EXPECT_EQ(opt->foo(), 18);
  }
  {
    bwOptional<TestUtilClasses::SimpleFunction> opt;
    EXPECT_DEATH((*opt).foo(), "");
  }
}
TEST(bwOptional, operator_arrow_const)
{
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunction> opt(in_place);
    EXPECT_EQ(opt->foo(), 23);
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunction> opt;
    ::testing::StaticAssertTypeEq<decltype(opt.operator->()),
                                  const TestUtilClasses::SimpleFunction*>();
  }
  {
    constexpr bwOptional<UndefinedAddressOfOperator> opt(in_place);
    EXPECT_EQ(opt->foo(), 18);
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunction> opt;
    EXPECT_DEATH((*opt).foo(), "");
  }
}

TEST(bwOptional, operator_dereference_lval_non_const)
{
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    EXPECT_EQ((*opt).foo(), TestUtilClasses::SimpleFunctionWithVariants::NON_CONST_LREF);
  }
  {
    bwOptional<TestUtilClasses::SimpleFunction> opt;
    ::testing::StaticAssertTypeEq<decltype(*opt), TestUtilClasses::SimpleFunction&>();
  }
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_DEATH((*opt).foo(), "attempt to get value of a disengaged optional");
  }
}
TEST(bwOptional, operator_dereference_lval_const)
{
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    MAYBE_STATIC_ASSERT((*opt).foo() == TestUtilClasses::SimpleFunctionWithVariants::CONST_LREF);
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunction> opt;
    ::testing::StaticAssertTypeEq<decltype(*opt), const TestUtilClasses::SimpleFunction&>();
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_DEATH((*opt).foo(), "attempt to get value of a disengaged optional");
  }
}

TEST(bwOptional, operator_dereference_rval_non_const)
{
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    EXPECT_EQ(std::move(*opt).foo(), TestUtilClasses::SimpleFunctionWithVariants::NON_CONST_RREF);
  }
  {
    bwOptional<TestUtilClasses::SimpleFunction> opt;
    ::testing::StaticAssertTypeEq<decltype(*std::move(opt)), TestUtilClasses::SimpleFunction&&>();
  }
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_DEATH(std::move(*opt).foo(), "attempt to get value of a disengaged optional");
  }
}
TEST(bwOptional, operator_dereference_rval_const)
{
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    MAYBE_STATIC_ASSERT(std::move(*opt).foo() ==
                        TestUtilClasses::SimpleFunctionWithVariants::CONST_RREF);
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunction> opt;
    ::testing::StaticAssertTypeEq<decltype(*std::move(opt)),
                                  const TestUtilClasses::SimpleFunction&&>();
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_DEATH(std::move(*opt).foo(), "attempt to get value of a disengaged optional");
  }
}

TEST(bwOptional, operator_bool)
{
  // is_convertible actually checks if it's implictly convertible, which should be disallowed.
  static_assert(!std::is_convertible<bwOptional<int>, bool>::value, "");
  {
    constexpr bwOptional<int> opt;
    MAYBE_STATIC_ASSERT(!opt);
  }
  {
    constexpr bwOptional<int> opt(-1);
    MAYBE_STATIC_ASSERT(opt);
  }
}
