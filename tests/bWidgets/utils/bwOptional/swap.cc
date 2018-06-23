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


class X
{
	int i_;
public:
	static int dtor_called;
	static int move_ctor_called;
	static int move_assign_called;

	X(int i) : i_(i) {}
	X(X&& other) :
	    i_(other.i_)
	{
		move_ctor_called++;
	}
	X& operator=(X&& other)
	{
		i_ = other.i_;
		move_assign_called++;
		return *this;
	}
	~X()
	{
		dtor_called++;
	}

	friend bool operator==(const X& x, const X& y)
	{
		return x.i_ == y.i_;
	}
};

int X::dtor_called = 0;
int X::move_ctor_called = 0;
int X::move_assign_called = 0;

class Y
{
	int i_;
public:
	static int dtor_called;
	static int swap_called;

	Y(int i) : i_(i) {}
	Y(Y&&) = default;
	~Y()
	{
		dtor_called++;
	}

	friend constexpr bool operator==(const Y& x, const Y& y)
	{
		return x.i_ == y.i_;
	}
	friend void swap(Y& x, Y& y)
	{
		std::swap(x.i_, y.i_);
		swap_called++;
	}
};

int Y::dtor_called = 0;
int Y::swap_called = 0;

TEST(bwOptional, swap_optionals)
{
	{
		bwOptional<int> opt1;
		bwOptional<int> opt2;
		EXPECT_FALSE(opt1);
		EXPECT_FALSE(opt2);
		swap(opt1, opt2);
		EXPECT_FALSE(opt1);
		EXPECT_FALSE(opt2);
	}
	{
		bwOptional<int> opt1(1);
		bwOptional<int> opt2;
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 1);
		EXPECT_FALSE(opt2);
		swap(opt1, opt2);
		EXPECT_FALSE(opt1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 1);
	}
	{
		bwOptional<int> opt1;
		bwOptional<int> opt2(2);
		EXPECT_FALSE(opt1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 2);
		swap(opt1, opt2);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 2);
		EXPECT_FALSE(opt2);
	}
	{
		bwOptional<int> opt1(1);
		bwOptional<int> opt2(2);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 2);
		swap(opt1, opt2);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 2);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 1);
	}
	{
		bwOptional<X> opt1;
		bwOptional<X> opt2;
		EXPECT_FALSE(opt1);
		EXPECT_FALSE(opt1);
		X::dtor_called = 0;
		X::move_ctor_called = 0;
		X::move_assign_called = 0;
		swap(opt1, opt2);
		EXPECT_FALSE(opt1);
		EXPECT_FALSE(opt2);
		EXPECT_EQ(X::dtor_called, 0);
		EXPECT_EQ(X::move_ctor_called, 0);
		EXPECT_EQ(X::move_assign_called, 0);
	}
	{
		bwOptional<X> opt1(1);
		bwOptional<X> opt2;
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 1);
		EXPECT_FALSE(opt2);
		X::dtor_called = 0;
		X::move_ctor_called = 0;
		X::move_assign_called = 0;
		swap(opt1, opt2);
		EXPECT_EQ(X::dtor_called, 1);
		EXPECT_EQ(X::move_ctor_called, 1);
		EXPECT_EQ(X::move_assign_called, 0);
		EXPECT_FALSE(opt1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 1);
	}
	{
		bwOptional<X> opt1;
		bwOptional<X> opt2(2);
		EXPECT_FALSE(opt1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 2);
		X::dtor_called = 0;
		X::move_ctor_called = 0;
		X::move_assign_called = 0;
		swap(opt1, opt2);
		EXPECT_EQ(X::dtor_called, 1);
		EXPECT_EQ(X::move_ctor_called, 1);
		EXPECT_EQ(X::move_assign_called, 0);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 2);
		EXPECT_FALSE(opt2);
	}
	{
		bwOptional<X> opt1(1);
		bwOptional<X> opt2(2);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 2);
		X::dtor_called = 0;
		X::move_ctor_called = 0;
		X::move_assign_called = 0;
		swap(opt1, opt2);
		EXPECT_EQ(X::dtor_called, 1);  // from inside std::swap
		EXPECT_EQ(X::move_ctor_called, 1);
		EXPECT_EQ(X::move_assign_called, 2);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 2);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 1);
	}
	{
		bwOptional<Y> opt1;
		bwOptional<Y> opt2;
		EXPECT_FALSE(opt1);
		EXPECT_FALSE(opt2);
		Y::swap_called = 0;
		swap(opt1, opt2);
		EXPECT_FALSE(opt1);
		EXPECT_FALSE(opt2);
		EXPECT_EQ(Y::dtor_called, 0);
		EXPECT_EQ(Y::swap_called, 0);
	}
	{
		bwOptional<Y> opt1(1);
		bwOptional<Y> opt2;
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 1);
		EXPECT_FALSE(opt2);
		Y::dtor_called = 0;
		Y::swap_called = 0;
		swap(opt1, opt2);
		EXPECT_EQ(Y::dtor_called, 1);
		EXPECT_EQ(Y::swap_called, 0);
		EXPECT_FALSE(opt1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 1);
	}
	{
		bwOptional<Y> opt1;
		bwOptional<Y> opt2(2);
		EXPECT_FALSE(opt1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 2);
		Y::dtor_called = 0;
		Y::swap_called = 0;
		swap(opt1, opt2);
		EXPECT_EQ(Y::dtor_called, 1);
		EXPECT_EQ(Y::swap_called, 0);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 2);
		EXPECT_FALSE(opt2);
	}
	{
		bwOptional<Y> opt1(1);
		bwOptional<Y> opt2(2);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 1);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 2);
		Y::dtor_called = 0;
		Y::swap_called = 0;
		swap(opt1, opt2);
		EXPECT_EQ(Y::dtor_called, 0);
		EXPECT_EQ(Y::swap_called, 1);
		EXPECT_TRUE(opt1);
		EXPECT_EQ(*opt1, 2);
		EXPECT_TRUE(opt2);
		EXPECT_EQ(*opt2, 1);
	}
}
