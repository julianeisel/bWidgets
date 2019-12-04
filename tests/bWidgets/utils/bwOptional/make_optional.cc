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

TEST(bwOptional, make_optional_implicit)
{
  {
    int arr[10];
    ((void)arr);
    ::testing::StaticAssertTypeEq<decltype(make_optional(arr)), bwOptional<int *>>();
  }
  {
    constexpr auto opt = make_optional(2);
    ::testing::StaticAssertTypeEq<decltype(opt), const bwOptional<int>>();
    MAYBE_STATIC_ASSERT(opt.value() == 2);
  }
  {
    bwOptional<int> opt = make_optional(2);
    EXPECT_EQ(*opt, 2);
  }
  {
    std::string s("123");
    bwOptional<std::string> opt = make_optional(s);
    EXPECT_EQ(*opt, s);
  }
  {
    std::unique_ptr<int> s(new int(3));
    bwOptional<std::unique_ptr<int>> opt = make_optional(std::move(s));
    EXPECT_EQ(**opt, 3);
    EXPECT_EQ(s, nullptr);
  }
}

TEST(bwOptional, make_optional_explicit)
{
  {
    constexpr auto opt = make_optional<int>('a');
    MAYBE_STATIC_ASSERT(*opt == int('a'));
  }
  {
    std::string s("123");
    auto opt = make_optional<std::string>(s);
    EXPECT_EQ(*opt, s);
  }
  {
    std::unique_ptr<int> s(new int(3));
    auto opt = make_optional<std::unique_ptr<int>>(std::move(s));
    EXPECT_EQ(**opt, 3);
    EXPECT_EQ(s, nullptr);
  }
  {
    auto opt = make_optional<std::string>(4, 'X');
    EXPECT_EQ(*opt, "XXXX");
  }
}

class TestT {
 public:
  int x;
  int size;

  constexpr TestT(std::initializer_list<int> il)
      : x(*il.begin()), size(static_cast<int>(il.size()))
  {
  }
  constexpr TestT(std::initializer_list<int> il, const int *)
      : x(*il.begin()), size(static_cast<int>(il.size()))
  {
  }
};

TEST(bwOptional, make_optional_explicit_with_init_list)
{
  {
    constexpr auto opt = make_optional<TestT>({42, 2, 3});
    ::testing::StaticAssertTypeEq<decltype(opt), const bwOptional<TestT>>();
    MAYBE_STATIC_ASSERT((*opt).x == 42);
    MAYBE_STATIC_ASSERT((*opt).size == 3);
  }
  {
    constexpr auto opt = make_optional<TestT>({42, 2, 3}, nullptr);
    MAYBE_STATIC_ASSERT((*opt).x == 42);
    MAYBE_STATIC_ASSERT((*opt).size == 3);
  }
  {
    auto opt = make_optional<std::string>({'1', '2', '3'});
    EXPECT_EQ(*opt, "123");
  }
  {
    auto opt = make_optional<std::string>({'a', 'b', 'c'}, std::allocator<char>{});
    EXPECT_EQ(*opt, "abc");
  }
}
