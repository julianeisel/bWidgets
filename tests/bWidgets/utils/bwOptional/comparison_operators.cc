/*
 * Much of this is based on Clang's implementation of std::optional. For fair
 * credit and copyright compliance, the following notice additionally applies:
 *
 * Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT (in
 * the directory containing bwOptional.h)
 */

#include "gtest/gtest.h"

#include "bwOptional.h"

#include "TestUtilMacros.h"

using namespace bWidgets;

struct X {
  int i_;
  constexpr X(int i) : i_(i)
  {
  }
};

constexpr bool operator==(const X& lhs, const X& rhs)
{
  return lhs.i_ == rhs.i_;
}
constexpr bool operator!=(const X& lhs, const X& rhs)
{
  return lhs.i_ != rhs.i_;
}
constexpr bool operator<(const X& lhs, const X& rhs)
{
  return lhs.i_ < rhs.i_;
}
constexpr bool operator<=(const X& lhs, const X& rhs)
{
  return lhs.i_ <= rhs.i_;
}
constexpr bool operator>(const X& lhs, const X& rhs)
{
  return lhs.i_ > rhs.i_;
}
constexpr bool operator>=(const X& lhs, const X& rhs)
{
  return lhs.i_ >= rhs.i_;
}

TEST(bwOptional, operator_equal)
{
  {
    constexpr bwOptional<X> o1;     // disengaged
    constexpr bwOptional<X> o2;     // disengaged
    constexpr bwOptional<X> o3{1};  // engaged
    constexpr bwOptional<X> o4{2};  // engaged
    constexpr bwOptional<X> o5{1};  // engaged

    MAYBE_STATIC_ASSERT(o1 == o1);
    MAYBE_STATIC_ASSERT(o1 == o2);
    MAYBE_STATIC_ASSERT(!(o1 == o3));
    MAYBE_STATIC_ASSERT(!(o1 == o4));
    MAYBE_STATIC_ASSERT(!(o1 == o5));

    MAYBE_STATIC_ASSERT(o2 == o1);
    MAYBE_STATIC_ASSERT(o2 == o2);
    MAYBE_STATIC_ASSERT(!(o2 == o3));
    MAYBE_STATIC_ASSERT(!(o2 == o4));
    MAYBE_STATIC_ASSERT(!(o2 == o5));

    MAYBE_STATIC_ASSERT(!(o3 == o1));
    MAYBE_STATIC_ASSERT(!(o3 == o2));
    MAYBE_STATIC_ASSERT(o3 == o3);
    MAYBE_STATIC_ASSERT(!(o3 == o4));
    MAYBE_STATIC_ASSERT(o3 == o5);

    MAYBE_STATIC_ASSERT(!(o4 == o1));
    MAYBE_STATIC_ASSERT(!(o4 == o2));
    MAYBE_STATIC_ASSERT(!(o4 == o3));
    MAYBE_STATIC_ASSERT(o4 == o4);
    MAYBE_STATIC_ASSERT(!(o4 == o5));

    MAYBE_STATIC_ASSERT(!(o5 == o1));
    MAYBE_STATIC_ASSERT(!(o5 == o2));
    MAYBE_STATIC_ASSERT(o5 == o3);
    MAYBE_STATIC_ASSERT(!(o5 == o4));
    MAYBE_STATIC_ASSERT(o5 == o5);
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 == bwOptional<long>(42));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(101) == o1));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 == bwOptional<const int>(42));
    MAYBE_STATIC_ASSERT(!(bwOptional<const int>(101) == o1));
  }
}

TEST(bwOptional, operator_not_equal)
{
  {
    constexpr bwOptional<X> o1;     // disengaged
    constexpr bwOptional<X> o2;     // disengaged
    constexpr bwOptional<X> o3{1};  // engaged
    constexpr bwOptional<X> o4{2};  // engaged
    constexpr bwOptional<X> o5{1};  // engaged

    MAYBE_STATIC_ASSERT(!(o1 != o1));
    MAYBE_STATIC_ASSERT(!(o1 != o2));
    MAYBE_STATIC_ASSERT((o1 != o3));
    MAYBE_STATIC_ASSERT((o1 != o4));
    MAYBE_STATIC_ASSERT((o1 != o5));

    MAYBE_STATIC_ASSERT(!(o2 != o1));
    MAYBE_STATIC_ASSERT(!(o2 != o2));
    MAYBE_STATIC_ASSERT((o2 != o3));
    MAYBE_STATIC_ASSERT((o2 != o4));
    MAYBE_STATIC_ASSERT((o2 != o5));

    MAYBE_STATIC_ASSERT((o3 != o1));
    MAYBE_STATIC_ASSERT((o3 != o2));
    MAYBE_STATIC_ASSERT(!(o3 != o3));
    MAYBE_STATIC_ASSERT((o3 != o4));
    MAYBE_STATIC_ASSERT(!(o3 != o5));

    MAYBE_STATIC_ASSERT((o4 != o1));
    MAYBE_STATIC_ASSERT((o4 != o2));
    MAYBE_STATIC_ASSERT((o4 != o3));
    MAYBE_STATIC_ASSERT(!(o4 != o4));
    MAYBE_STATIC_ASSERT((o4 != o5));

    MAYBE_STATIC_ASSERT((o5 != o1));
    MAYBE_STATIC_ASSERT((o5 != o2));
    MAYBE_STATIC_ASSERT(!(o5 != o3));
    MAYBE_STATIC_ASSERT((o5 != o4));
    MAYBE_STATIC_ASSERT(!(o5 != o5));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 != bwOptional<long>(101));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(42) != o1));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 != bwOptional<const int>(101));
    MAYBE_STATIC_ASSERT(!(bwOptional<const int>(42) != o1));
  }
}

TEST(bwOptional, operator_less_than)
{
  {
    constexpr bwOptional<X> o1;     // disengaged
    constexpr bwOptional<X> o2;     // disengaged
    constexpr bwOptional<X> o3{1};  // engaged
    constexpr bwOptional<X> o4{2};  // engaged
    constexpr bwOptional<X> o5{1};  // engaged

    MAYBE_STATIC_ASSERT(!(o1 < o1));
    MAYBE_STATIC_ASSERT(!(o1 < o2));
    MAYBE_STATIC_ASSERT((o1 < o3));
    MAYBE_STATIC_ASSERT((o1 < o4));
    MAYBE_STATIC_ASSERT((o1 < o5));

    MAYBE_STATIC_ASSERT(!(o2 < o1));
    MAYBE_STATIC_ASSERT(!(o2 < o2));
    MAYBE_STATIC_ASSERT((o2 < o3));
    MAYBE_STATIC_ASSERT((o2 < o4));
    MAYBE_STATIC_ASSERT((o2 < o5));

    MAYBE_STATIC_ASSERT(!(o3 < o1));
    MAYBE_STATIC_ASSERT(!(o3 < o2));
    MAYBE_STATIC_ASSERT(!(o3 < o3));
    MAYBE_STATIC_ASSERT((o3 < o4));
    MAYBE_STATIC_ASSERT(!(o3 < o5));

    MAYBE_STATIC_ASSERT(!(o4 < o1));
    MAYBE_STATIC_ASSERT(!(o4 < o2));
    MAYBE_STATIC_ASSERT(!(o4 < o3));
    MAYBE_STATIC_ASSERT(!(o4 < o4));
    MAYBE_STATIC_ASSERT(!(o4 < o5));

    MAYBE_STATIC_ASSERT(!(o5 < o1));
    MAYBE_STATIC_ASSERT(!(o5 < o2));
    MAYBE_STATIC_ASSERT(!(o5 < o3));
    MAYBE_STATIC_ASSERT((o5 < o4));
    MAYBE_STATIC_ASSERT(!(o5 < o5));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 < bwOptional<long>(101));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(101) < o1));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 < bwOptional<long>(101));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(101) < o1));
  }
}

TEST(bwOptional, operator_less_than_or_equal)
{
  {
    constexpr bwOptional<X> o1;     // disengaged
    constexpr bwOptional<X> o2;     // disengaged
    constexpr bwOptional<X> o3{1};  // engaged
    constexpr bwOptional<X> o4{2};  // engaged
    constexpr bwOptional<X> o5{1};  // engaged

    MAYBE_STATIC_ASSERT((o1 <= o1));
    MAYBE_STATIC_ASSERT((o1 <= o2));
    MAYBE_STATIC_ASSERT((o1 <= o3));
    MAYBE_STATIC_ASSERT((o1 <= o4));
    MAYBE_STATIC_ASSERT((o1 <= o5));

    MAYBE_STATIC_ASSERT((o2 <= o1));
    MAYBE_STATIC_ASSERT((o2 <= o2));
    MAYBE_STATIC_ASSERT((o2 <= o3));
    MAYBE_STATIC_ASSERT((o2 <= o4));
    MAYBE_STATIC_ASSERT((o2 <= o5));

    MAYBE_STATIC_ASSERT(!(o3 <= o1));
    MAYBE_STATIC_ASSERT(!(o3 <= o2));
    MAYBE_STATIC_ASSERT((o3 <= o3));
    MAYBE_STATIC_ASSERT((o3 <= o4));
    MAYBE_STATIC_ASSERT((o3 <= o5));

    MAYBE_STATIC_ASSERT(!(o4 <= o1));
    MAYBE_STATIC_ASSERT(!(o4 <= o2));
    MAYBE_STATIC_ASSERT(!(o4 <= o3));
    MAYBE_STATIC_ASSERT((o4 <= o4));
    MAYBE_STATIC_ASSERT(!(o4 <= o5));

    MAYBE_STATIC_ASSERT(!(o5 <= o1));
    MAYBE_STATIC_ASSERT(!(o5 <= o2));
    MAYBE_STATIC_ASSERT((o5 <= o3));
    MAYBE_STATIC_ASSERT((o5 <= o4));
    MAYBE_STATIC_ASSERT((o5 <= o5));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 <= bwOptional<long>(42));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(101) <= o1));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 <= bwOptional<const int>(42));
    MAYBE_STATIC_ASSERT(!(bwOptional<const int>(101) <= o1));
  }
}

TEST(bwOptional, operator_greater_than)
{
  {
    constexpr bwOptional<X> o1;     // disengaged
    constexpr bwOptional<X> o2;     // disengaged
    constexpr bwOptional<X> o3{1};  // engaged
    constexpr bwOptional<X> o4{2};  // engaged
    constexpr bwOptional<X> o5{1};  // engaged

    MAYBE_STATIC_ASSERT(!(o1 > o1));
    MAYBE_STATIC_ASSERT(!(o1 > o2));
    MAYBE_STATIC_ASSERT(!(o1 > o3));
    MAYBE_STATIC_ASSERT(!(o1 > o4));
    MAYBE_STATIC_ASSERT(!(o1 > o5));

    MAYBE_STATIC_ASSERT(!(o2 > o1));
    MAYBE_STATIC_ASSERT(!(o2 > o2));
    MAYBE_STATIC_ASSERT(!(o2 > o3));
    MAYBE_STATIC_ASSERT(!(o2 > o4));
    MAYBE_STATIC_ASSERT(!(o2 > o5));

    MAYBE_STATIC_ASSERT((o3 > o1));
    MAYBE_STATIC_ASSERT((o3 > o2));
    MAYBE_STATIC_ASSERT(!(o3 > o3));
    MAYBE_STATIC_ASSERT(!(o3 > o4));
    MAYBE_STATIC_ASSERT(!(o3 > o5));

    MAYBE_STATIC_ASSERT((o4 > o1));
    MAYBE_STATIC_ASSERT((o4 > o2));
    MAYBE_STATIC_ASSERT((o4 > o3));
    MAYBE_STATIC_ASSERT(!(o4 > o4));
    MAYBE_STATIC_ASSERT((o4 > o5));

    MAYBE_STATIC_ASSERT((o5 > o1));
    MAYBE_STATIC_ASSERT((o5 > o2));
    MAYBE_STATIC_ASSERT(!(o5 > o3));
    MAYBE_STATIC_ASSERT(!(o5 > o4));
    MAYBE_STATIC_ASSERT(!(o5 > o5));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 > bwOptional<long>(1));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(42) > o1));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 > bwOptional<const int>(1));
    MAYBE_STATIC_ASSERT(!(bwOptional<const int>(42) > o1));
  }
}

TEST(bwOptional, operator_greater_than_or_equal)
{
  {
    constexpr bwOptional<X> o1;     // disengaged
    constexpr bwOptional<X> o2;     // disengaged
    constexpr bwOptional<X> o3{1};  // engaged
    constexpr bwOptional<X> o4{2};  // engaged
    constexpr bwOptional<X> o5{1};  // engaged

    MAYBE_STATIC_ASSERT((o1 >= o1));
    MAYBE_STATIC_ASSERT((o1 >= o2));
    MAYBE_STATIC_ASSERT(!(o1 >= o3));
    MAYBE_STATIC_ASSERT(!(o1 >= o4));
    MAYBE_STATIC_ASSERT(!(o1 >= o5));

    MAYBE_STATIC_ASSERT((o2 >= o1));
    MAYBE_STATIC_ASSERT((o2 >= o2));
    MAYBE_STATIC_ASSERT(!(o2 >= o3));
    MAYBE_STATIC_ASSERT(!(o2 >= o4));
    MAYBE_STATIC_ASSERT(!(o2 >= o5));

    MAYBE_STATIC_ASSERT((o3 >= o1));
    MAYBE_STATIC_ASSERT((o3 >= o2));
    MAYBE_STATIC_ASSERT((o3 >= o3));
    MAYBE_STATIC_ASSERT(!(o3 >= o4));
    MAYBE_STATIC_ASSERT((o3 >= o5));

    MAYBE_STATIC_ASSERT((o4 >= o1));
    MAYBE_STATIC_ASSERT((o4 >= o2));
    MAYBE_STATIC_ASSERT((o4 >= o3));
    MAYBE_STATIC_ASSERT((o4 >= o4));
    MAYBE_STATIC_ASSERT((o4 >= o5));

    MAYBE_STATIC_ASSERT((o5 >= o1));
    MAYBE_STATIC_ASSERT((o5 >= o2));
    MAYBE_STATIC_ASSERT((o5 >= o3));
    MAYBE_STATIC_ASSERT(!(o5 >= o4));
    MAYBE_STATIC_ASSERT((o5 >= o5));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 >= bwOptional<long>(42));
    MAYBE_STATIC_ASSERT(!(bwOptional<long>(11) >= o1));
  }
  {
    constexpr bwOptional<int> o1(42);
    MAYBE_STATIC_ASSERT(o1 >= bwOptional<const int>(42));
    MAYBE_STATIC_ASSERT(!(bwOptional<const int>(1) >= o1));
  }
}
