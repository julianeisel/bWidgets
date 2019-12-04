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

TEST(bwOptional, has_value)
{
  {
    constexpr bwOptional<int> opt;
    ::testing::StaticAssertTypeEq<decltype(opt.has_value()), bool>();
  }
  {
    constexpr bwOptional<int> opt;
    MAYBE_STATIC_ASSERT(!opt.has_value());
  }
  {
    constexpr bwOptional<int> opt(-1);
    MAYBE_STATIC_ASSERT(opt.has_value());
  }
}

TEST(bwOptional, value_lval_non_const)
{
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    ::testing::StaticAssertTypeEq<decltype(opt.value()),
                                  TestUtilClasses::SimpleFunctionWithVariants &>();
  }
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    EXPECT_EQ(opt.value().foo(), TestUtilClasses::SimpleFunctionWithVariants::NON_CONST_LREF);
  }
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_THROW(opt.value(), bad_optional_access);
  }
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(15);
    EXPECT_EQ(opt.value()._value, 15);
  }
}
TEST(bwOptional, value_lval_const)
{
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    ::testing::StaticAssertTypeEq<decltype(opt.value()),
                                  const TestUtilClasses::SimpleFunctionWithVariants &>();
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    MAYBE_STATIC_ASSERT(opt.value().foo() ==
                        TestUtilClasses::SimpleFunctionWithVariants::CONST_LREF);
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_THROW(opt.value(), bad_optional_access);
  }
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt(21);
    EXPECT_EQ(opt.value()._value, 21);
  }
}
TEST(bwOptional, value_rval_non_const)
{
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    ::testing::StaticAssertTypeEq<decltype(std::move(opt).value()),
                                  TestUtilClasses::SimpleFunctionWithVariants &&>();
  }
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    EXPECT_EQ(std::move(opt).value().foo(),
              TestUtilClasses::SimpleFunctionWithVariants::NON_CONST_RREF);
  }
  {
    bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_THROW(std::move(opt).value(), bad_optional_access);
  }
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(21);
    EXPECT_EQ(std::move(opt).value()._value, 21);
  }
}
TEST(bwOptional, value_rval_const)
{
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    ::testing::StaticAssertTypeEq<decltype(std::move(opt).value()),
                                  const TestUtilClasses::SimpleFunctionWithVariants &&>();
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt(in_place);
    EXPECT_EQ(std::move(opt).value().foo(),
              TestUtilClasses::SimpleFunctionWithVariants::CONST_RREF);
  }
  {
    constexpr bwOptional<TestUtilClasses::SimpleFunctionWithVariants> opt;
    EXPECT_THROW(std::move(opt).value(), bad_optional_access);
  }
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(21);
    EXPECT_EQ(std::move(opt).value()._value, 21);
  }
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt(30);
    EXPECT_EQ(std::move(opt).value()._value, 30);
  }
}

TEST(bwOptional, value_or_const_lref)
{
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt;

    EXPECT_EQ(opt.value_or(TestUtilClasses::NonTrivialConstructors(5))._value, 5);
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt;

    EXPECT_EQ(opt.value_or(5)._value, 5);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    EXPECT_FALSE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    TestUtilClasses::NonTrivialConstructors data(10);

    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(opt.value_or(data)._value, 10);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    EXPECT_FALSE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt(in_place);

    EXPECT_EQ(opt.value_or(TestUtilClasses::NonTrivialConstructors(5))._value, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    EXPECT_TRUE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    const bwOptional<TestUtilClasses::NonTrivialConstructors> opt(2);

    EXPECT_EQ(opt.value_or(5)._value, 2);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    EXPECT_TRUE(opt);
  }

  {
    constexpr bwOptional<TestUtilClasses::ConstexprConstructors> opt;
    MAYBE_STATIC_ASSERT(opt.value_or(10)._value == 10);
  }
  {
    constexpr bwOptional<TestUtilClasses::ConstexprConstructors> opt(3);
    MAYBE_STATIC_ASSERT(opt.value_or(10)._value == 3);
  }
}

TEST(bwOptional, value_or_non_const_rref)
{
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;

    EXPECT_EQ(std::move(opt).value_or(TestUtilClasses::NonTrivialConstructors(5))._value, 5);
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;

    EXPECT_EQ(std::move(opt).value_or(5)._value, 5);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    EXPECT_FALSE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    TestUtilClasses::NonTrivialConstructors data(10);

    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(std::move(opt).value_or(data)._value, 10);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    EXPECT_FALSE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(in_place);

    EXPECT_EQ(std::move(opt).value_or(TestUtilClasses::NonTrivialConstructors(5))._value, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    EXPECT_TRUE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(2);

    EXPECT_EQ(std::move(opt).value_or(5)._value, 2);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
    EXPECT_TRUE(opt);
  }
}

TEST(bwOptional, swap)
{
  {
    bwOptional<int> opt1, opt2;
    EXPECT_FALSE(opt1);
    EXPECT_FALSE(opt2);
    opt1.swap(opt2);
    EXPECT_FALSE(opt1);
    EXPECT_FALSE(opt2);
  }
  {
    bwOptional<int> opt1(3), opt2;
    EXPECT_TRUE(opt1);
    EXPECT_FALSE(opt2);
    EXPECT_EQ(opt1.value(), 3);
    opt1.swap(opt2);
    EXPECT_FALSE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt2.value(), 3);
    EXPECT_THROW(opt1.value(), bad_optional_access);
  }
  {
    bwOptional<int> opt1, opt2(2);
    EXPECT_FALSE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt2.value(), 2);
    opt1.swap(opt2);
    EXPECT_TRUE(opt1);
    EXPECT_FALSE(opt2);
    EXPECT_EQ(opt1.value(), 2);
    EXPECT_THROW(opt2.value(), bad_optional_access);
  }
  {
    bwOptional<int> opt1(4), opt2(2);
    EXPECT_TRUE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt1.value(), 4);
    EXPECT_EQ(opt2.value(), 2);
    opt1.swap(opt2);
    EXPECT_TRUE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt1.value(), 2);
    EXPECT_EQ(opt2.value(), 4);
  }

  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt1, opt2;
    EXPECT_FALSE(opt1);
    EXPECT_FALSE(opt2);
    opt1.swap(opt2);
    EXPECT_FALSE(opt1);
    EXPECT_FALSE(opt2);
    TestUtilClasses::NonTrivialConstructors::assert_no_instance();
  }
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt1(20), opt2;
    EXPECT_TRUE(opt1);
    EXPECT_FALSE(opt2);
    EXPECT_EQ(opt1.value()._value, 20);
    opt1.swap(opt2);
    EXPECT_FALSE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt2.value()._value, 20);
    EXPECT_THROW(opt1.value(), bad_optional_access);
    EXPECT_EQ(opt2.value().tot_default_ctor_calls, 0);
    EXPECT_EQ(opt2.value().tot_copy_ctor_calls, 0);
    EXPECT_EQ(opt2.value().tot_move_ctor_calls, 1);
    EXPECT_EQ(opt2.value().tot_int_ctor_calls, 1);
    EXPECT_EQ(opt2.value().tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt1, opt2(30);
    EXPECT_FALSE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt2.value()._value, 30);
    opt1.swap(opt2);
    EXPECT_TRUE(opt1);
    EXPECT_FALSE(opt2);
    EXPECT_EQ(opt1.value()._value, 30);
    EXPECT_THROW(opt2.value(), bad_optional_access);
    EXPECT_EQ(opt1.value().tot_default_ctor_calls, 0);
    EXPECT_EQ(opt1.value().tot_copy_ctor_calls, 0);
    EXPECT_EQ(opt1.value().tot_move_ctor_calls, 1);
    EXPECT_EQ(opt1.value().tot_int_ctor_calls, 1);
    EXPECT_EQ(opt1.value().tot_instances, 1);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt1(42), opt2(30);
    EXPECT_TRUE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt1.value()._value, 42);
    EXPECT_EQ(opt2.value()._value, 30);
    opt1.swap(opt2);
    EXPECT_TRUE(opt1);
    EXPECT_TRUE(opt2);
    EXPECT_EQ(opt1.value()._value, 30);
    EXPECT_EQ(opt2.value()._value, 42);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 0);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 2);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 2);
  }

  {
    using _T = TestUtilClasses::CustomSwap;
    {
      _T::reset();
      bwOptional<_T> opt1, opt2;
      EXPECT_FALSE(opt1);
      EXPECT_FALSE(opt2);
      opt1.swap(opt2);
      EXPECT_FALSE(opt1);
      EXPECT_FALSE(opt2);
      _T::assert_no_instance();
    }
    {
      _T::reset();
      bwOptional<_T> opt1(20), opt2;
      EXPECT_TRUE(opt1);
      EXPECT_FALSE(opt2);
      EXPECT_EQ(opt1.value()._value, 20);
      opt1.swap(opt2);
      EXPECT_FALSE(opt1);
      EXPECT_TRUE(opt2);
      EXPECT_EQ(opt2.value()._value, 20);
      EXPECT_THROW(opt1.value(), bad_optional_access);
      EXPECT_EQ(_T::tot_int_ctor_calls, 1);
      EXPECT_EQ(_T::tot_move_ctor_calls, 1);
      EXPECT_EQ(_T::tot_dtor_calls, 1);
      EXPECT_EQ(_T::tot_swap_calls, 0);
    }
    {
      _T::reset();
      bwOptional<_T> opt1, opt2(30);
      EXPECT_FALSE(opt1);
      EXPECT_TRUE(opt2);
      EXPECT_EQ(opt2.value()._value, 30);
      opt1.swap(opt2);
      EXPECT_TRUE(opt1);
      EXPECT_FALSE(opt2);
      EXPECT_EQ(opt1.value()._value, 30);
      EXPECT_THROW(opt2.value(), bad_optional_access);
      EXPECT_EQ(_T::tot_int_ctor_calls, 1);
      EXPECT_EQ(_T::tot_move_ctor_calls, 1);
      EXPECT_EQ(_T::tot_dtor_calls, 1);
      EXPECT_EQ(_T::tot_swap_calls, 0);
    }
    {
      _T::reset();
      bwOptional<_T> opt1(50), opt2(30);
      EXPECT_TRUE(opt1);
      EXPECT_TRUE(opt2);
      EXPECT_EQ(opt1.value()._value, 50);
      EXPECT_EQ(opt2.value()._value, 30);
      opt1.swap(opt2);
      EXPECT_TRUE(opt1);
      EXPECT_TRUE(opt2);
      EXPECT_EQ(opt1.value()._value, 30);
      EXPECT_EQ(opt2.value()._value, 50);
      EXPECT_EQ(_T::tot_int_ctor_calls, 2);
      EXPECT_EQ(_T::tot_move_ctor_calls, 0);
      EXPECT_EQ(_T::tot_dtor_calls, 0);
      EXPECT_EQ(_T::tot_swap_calls, 1);
    }
  }
}

TEST(bwOptional, reset)
{
  {
    bwOptional<int> opt;
    opt.reset();
    EXPECT_FALSE(opt);
  }
  {
    bwOptional<int> opt(42);
    EXPECT_TRUE(opt);
    opt.reset();
    EXPECT_FALSE(opt);
  }
  TestUtilClasses::NonTrivialConstructors::reset();
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_dtor_calls, 0);
    opt.reset();
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_dtor_calls, 0);
  }
  {
    bwOptional<TestUtilClasses::NonTrivialConstructors> opt(10);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_dtor_calls, 0);
    opt.reset();
    EXPECT_FALSE(opt);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_dtor_calls, 1);
    EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 0);
    EXPECT_THROW(opt.value(), bad_optional_access);
  }
}

template<typename _T> void emplace_test_with_type()
{
  _T::reset();
  bwOptional<_T> opt;
  _T::assert_no_instance();
  EXPECT_FALSE(opt);

  {
    _T::reset_constructors();
    _T &value = opt.emplace();

    EXPECT_EQ(_T::tot_default_ctor_calls, 1);
    EXPECT_EQ(_T::tot_dtor_calls, 0);
    EXPECT_EQ(_T::tot_instances, 1);
    EXPECT_TRUE(opt);
    EXPECT_EQ(value._value, 0);
    EXPECT_EQ(&value, &*opt);
  }
  {
    _T::reset_constructors();
    auto &value = opt.emplace();

    ::testing::StaticAssertTypeEq<_T &, decltype(value)>();
    EXPECT_EQ(_T::tot_instances, 1);
    EXPECT_EQ(_T::tot_default_ctor_calls, 1);
    EXPECT_EQ(_T::tot_dtor_calls, 1);
    EXPECT_TRUE(opt);
    EXPECT_EQ(value._value, 0);
    EXPECT_EQ(&value, &*opt);
  }
  {
    _T::reset_constructors();
    auto &value = opt.emplace(42);

    ::testing::StaticAssertTypeEq<_T &, decltype(value)>();
    EXPECT_EQ(_T::tot_instances, 1);
    EXPECT_EQ(_T::tot_default_ctor_calls, 0);
    EXPECT_EQ(_T::tot_int_ctor_calls, 1);
    EXPECT_EQ(_T::tot_dtor_calls, 2);
    EXPECT_TRUE(opt);
    EXPECT_EQ(value._value, 42);
    EXPECT_EQ(&value, &*opt);
  }
}
TEST(bwOptional, emplace)
{
  {
    emplace_test_with_type<TestUtilClasses::NonTrivialConstructors>();
    emplace_test_with_type<TestUtilClasses::ExplicitNonTrivialConstructors>();
  }

  {
    class MultiArg {
     public:
      MultiArg(int x, int y, int z) : _x(x), _y(y), _z(z)
      {
      }
      int _x, _y, _z;
    };
    {
      bwOptional<MultiArg> opt;
      auto &value = opt.emplace(5, 6, 7);

      ::testing::StaticAssertTypeEq<MultiArg &, decltype(value)>();
      EXPECT_EQ(value._x, 5);
      EXPECT_EQ(value._y, 6);
      EXPECT_EQ(value._z, 7);
    }
  }

  {
    bwOptional<const int> opt;
    const int &value = opt.emplace(3);
    EXPECT_EQ(*opt, 3);
    EXPECT_EQ(*opt, value);
    opt.emplace();
    EXPECT_EQ(*opt, 0);
  }
}

class InitListConstructible {
 public:
  InitListConstructible() = default;
  InitListConstructible(std::initializer_list<int> ilist)
      : _a(ilist.begin()[0]), _b(ilist.begin()[1])
  {
  }
  ~InitListConstructible()
  {
    tot_ctor_calls++;
  }

  int _a, _b;
  static int tot_ctor_calls;
};
int InitListConstructible::tot_ctor_calls = 0;

TEST(bwOptional, emplace_with_init_list)
{
  {
    bwOptional<InitListConstructible> opt(in_place);

    EXPECT_EQ(InitListConstructible::tot_ctor_calls, 0);
    auto &value = opt.emplace<int>({9, 2});
    ::testing::StaticAssertTypeEq<InitListConstructible &, decltype(value)>();
    EXPECT_EQ(InitListConstructible::tot_ctor_calls, 1);
    EXPECT_EQ(opt->_a, 9);
    EXPECT_EQ(opt->_b, 2);
    EXPECT_EQ(&value, &*opt);
  }
  {
    bwOptional<std::vector<int>> opt;
    auto &value = opt.emplace({3, 4, 5}, std::allocator<int>());

    ::testing::StaticAssertTypeEq<std::vector<int> &, decltype(value)>();
    EXPECT_TRUE(opt);
    EXPECT_EQ(*opt, std::vector<int>({3, 4, 5}));
    EXPECT_EQ(&value, &*opt);
  }
}
