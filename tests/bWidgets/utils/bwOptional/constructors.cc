/*
 * Much of this is based on Clang's implementation of std::optional. For fair
 * credit and copyright compliance, the following notice additionally applies:
 *
 * Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT (in
 * the directory containing bwOptional.h)
 */

#include <vector>

#include "gtest/gtest.h"

#include "bwOptional.h"

#include "TestUtilClasses.h"
#include "TestUtilMacros.h"

using namespace bWidgets;


template<typename _Optional>
void test_constructor_default_constexpr()
{
	static_assert(std::is_default_constructible<_Optional>::value, "");

	constexpr _Optional opt;
	MAYBE_STATIC_ASSERT(!opt);

	class test_constructor_default_inheritable : public _Optional
	{
		constexpr test_constructor_default_inheritable()
		{}
	};
}
TEST(bwOptional, constructor_default)
{
	test_constructor_default_constexpr<bwOptional<int>>();
	test_constructor_default_constexpr<bwOptional<int*>>();
	test_constructor_default_constexpr<bwOptional<TestUtilClasses::NoConstructors>>();

	// Test non-constexpr
	{
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
		EXPECT_FALSE(opt);
		TestUtilClasses::NonTrivialConstructors::assert_no_instance();
	}
}

template<typename _Optional>
void test_constructor_nullopt_constexpr()
{
	static_assert(std::is_constructible<_Optional, nullopt_t&>::value, "");

	constexpr _Optional opt(nullopt);
	MAYBE_STATIC_ASSERT(!opt);

	class test_constructor_default_inheritable : public _Optional
	{
		constexpr test_constructor_default_inheritable()
		{}
	};
}
TEST(bwOptional, constructor_nullopt)
{
	test_constructor_nullopt_constexpr<bwOptional<int>>();
	test_constructor_nullopt_constexpr<bwOptional<int*>>();
	test_constructor_nullopt_constexpr<bwOptional<TestUtilClasses::NoConstructors*>>();

	// Test non-constexpr
	{
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt{nullopt};
		EXPECT_FALSE(opt);
		TestUtilClasses::NonTrivialConstructors::assert_no_instance();
	}
}

TEST(bwOptional, constructor_copy)
{
	static_assert(std::is_trivially_copy_constructible<bwOptional<int>>::value, "");
	static_assert(!std::is_trivially_copy_constructible<bwOptional<TestUtilClasses::NonTrivialConstructors>>::value,
	              "");
	static_assert(!std::is_copy_constructible<bwOptional<TestUtilClasses::NoConstructors>>::value, "");

	{
		constexpr bwOptional<int> opt;
		constexpr bwOptional<int> opt_other(opt);

		MAYBE_STATIC_ASSERT(!opt_other);
	}
	{
		constexpr bwOptional<int> opt{42};
		constexpr bwOptional<int> opt_other(opt);

		MAYBE_STATIC_ASSERT(opt_other);
		MAYBE_STATIC_ASSERT(*opt_other == 42);
	}

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt;
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other = opt; // Calls copy constructor.

		EXPECT_FALSE(opt_other);
		TestUtilClasses::NonTrivialConstructors::assert_no_instance();
	}
	{
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt{42};
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt_other = opt; // Calls copy constructor.

		EXPECT_TRUE(opt_other);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls, 0);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls, 1);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 2);
		EXPECT_EQ(opt_other->_value, 42);
	}
}

TEST(bwOptional, constructor_move)
{
	{
		constexpr bwOptional<int> opt;
		constexpr bwOptional<int> opt_other(std::move(opt));

		MAYBE_STATIC_ASSERT(!opt_other);
	}
	{
		constexpr bwOptional<int> opt{42};
		constexpr bwOptional<int> opt_other(std::move(opt));

		MAYBE_STATIC_ASSERT(opt_other);
		MAYBE_STATIC_ASSERT(*opt_other == 42);
	}
	static_assert(!std::is_copy_constructible<bwOptional<TestUtilClasses::NoCopyConstructor>>::value, "");
	static_assert( std::is_move_constructible<bwOptional<TestUtilClasses::NoCopyConstructor>>::value, "");

	TestUtilClasses::NoCopyConstructor::reset();
	{
		bwOptional<TestUtilClasses::NoCopyConstructor> opt;
		bwOptional<TestUtilClasses::NoCopyConstructor>{std::move(opt)};

		TestUtilClasses::NoCopyConstructor::assert_no_instance();
	}
	{
		bwOptional<TestUtilClasses::NoCopyConstructor> opt{42};
		bwOptional<TestUtilClasses::NoCopyConstructor> opt_other{std::move(opt)};

		EXPECT_TRUE(opt_other);
		EXPECT_TRUE(opt);
		EXPECT_EQ(TestUtilClasses::NoCopyConstructor::tot_move_ctor_calls, 1);
		EXPECT_EQ(TestUtilClasses::NoCopyConstructor::tot_instances, 2);
		EXPECT_EQ(opt_other->_value, 42);
		EXPECT_EQ(opt->_value, -1);
	}

	static_assert(!std::is_copy_constructible<bwOptional<TestUtilClasses::NoConstructors>>::value, "");
	static_assert(!std::is_move_constructible<bwOptional<TestUtilClasses::NoConstructors>>::value, "");

	static_assert(std::is_copy_constructible<bwOptional<TestUtilClasses::NoMoveConstructor>>::value, "");
	// A type can be move-constructible if it's copy-constructible, even if move constructor is deleted.
	static_assert(std::is_move_constructible<bwOptional<TestUtilClasses::NoMoveConstructor>>::value, "");

	{
		struct HasTrivialMoveConstructor
		{
			HasTrivialMoveConstructor() = default;
			HasTrivialMoveConstructor(const HasTrivialMoveConstructor&) = delete;
			HasTrivialMoveConstructor(HasTrivialMoveConstructor&&) = default;
		};
		static_assert(std::is_trivially_move_constructible<HasTrivialMoveConstructor>::value, "");

		/* Copy constructor should somehow be excluded from overload resolution... */
		constexpr bwOptional<HasTrivialMoveConstructor> opt{bwOptional<HasTrivialMoveConstructor>()};
		MAYBE_STATIC_ASSERT(!opt);
		constexpr bwOptional<HasTrivialMoveConstructor> opt_other{
		        bwOptional<HasTrivialMoveConstructor>(HasTrivialMoveConstructor())};
		MAYBE_STATIC_ASSERT(opt_other);
	}
}

TEST(bwOptional, constructor_converting_copy_implicit)
{
	{
		bwOptional<short> rhs;
		bwOptional<int> lhs(rhs);
		EXPECT_FALSE(lhs);
	}
	{
		bwOptional<short> rhs(42);
		bwOptional<int> lhs(rhs);
		EXPECT_TRUE(lhs);
		EXPECT_EQ(*lhs, 42);
	}

	static_assert(std::is_constructible<TestUtilClasses::NonTrivialConstructors, const int&>::value, "");

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<int> rhs;
		bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(rhs);

		TestUtilClasses::NonTrivialConstructors::assert_no_instance();
		EXPECT_FALSE(lhs);
	}
	{
		bwOptional<int> rhs(42);
		bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(rhs);
		EXPECT_TRUE(lhs);
		EXPECT_EQ(lhs->_value, 42);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_instances, 1);
	}
}
TEST(bwOptional, constructor_converting_copy_explicit)
{
	static_assert(std::is_constructible<TestUtilClasses::ExplicitConstructibleFromInt, const int&>::value, "");
	static_assert(!std::is_convertible<const int&, TestUtilClasses::ExplicitConstructibleFromInt>::value, "");
	static_assert(!std::is_convertible<const bwOptional<int>&,
	              bwOptional<TestUtilClasses::ExplicitConstructibleFromInt>>::value, "");

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<int> rhs;
		bwOptional<TestUtilClasses::ExplicitConstructibleFromInt> lhs(rhs);

		EXPECT_FALSE(lhs);
		TestUtilClasses::ExplicitConstructibleFromInt::assert_no_instance();
	}
	{
		bwOptional<int> rhs(42);
		bwOptional<TestUtilClasses::ExplicitConstructibleFromInt> lhs(rhs);

		EXPECT_TRUE(lhs);
		EXPECT_EQ(lhs->_value, 42);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
	}
}

TEST(bwOptional, constructor_converting_move_implicit)
{
	{
		bwOptional<short> rhs;
		bwOptional<int> lhs(std::move(rhs));
		EXPECT_FALSE(lhs);
	}
	{
		bwOptional<short> rhs(42);
		bwOptional<int> lhs(std::move(rhs));
		EXPECT_TRUE(lhs);
		EXPECT_EQ(*lhs, 42);
	}

	static_assert(std::is_constructible<TestUtilClasses::NonTrivialConstructors, int&&>::value, "");

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<int> rhs;
		bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(std::move(rhs));

		TestUtilClasses::NonTrivialConstructors::assert_no_instance();
		EXPECT_FALSE(lhs);
	}
	{
		bwOptional<int> rhs(42);
		bwOptional<TestUtilClasses::NonTrivialConstructors> lhs(std::move(rhs));
		EXPECT_TRUE(lhs);
		EXPECT_EQ(lhs->_value, 42);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
	}
}

TEST(bwOptional, constructor_converting_move_explicit)
{
	static_assert(std::is_constructible<TestUtilClasses::ExplicitConstructibleFromInt, int&&>::value, "");
	static_assert(!std::is_convertible<const int&, TestUtilClasses::ExplicitConstructibleFromInt>::value, "");
	static_assert(!std::is_convertible<const bwOptional<int>&, bwOptional<TestUtilClasses::ExplicitConstructibleFromInt>>::value, "");

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<int> rhs;
		bwOptional<TestUtilClasses::ExplicitConstructibleFromInt> lhs(std::move(rhs));

		EXPECT_FALSE(lhs);
		TestUtilClasses::ExplicitConstructibleFromInt::assert_no_instance();
	}
	{
		bwOptional<int> rhs(42);
		bwOptional<TestUtilClasses::ExplicitConstructibleFromInt> lhs(std::move(rhs));

		EXPECT_TRUE(lhs);
		EXPECT_EQ(TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls, 1);
		EXPECT_EQ(lhs->_value, 42);
	}
}

TEST(bwOptional, constructor_in_place)
{
	{
		bwOptional<int> opt{in_place, 10};

		EXPECT_TRUE(opt);
		EXPECT_EQ(*opt, 10);
	}
	{
		constexpr bwOptional<int> opt{in_place, 20};

		MAYBE_STATIC_ASSERT(opt);
		MAYBE_STATIC_ASSERT(*opt == 20);
	}
	{
		constexpr bwOptional<const int> opt{in_place, 30};

		MAYBE_STATIC_ASSERT(opt);
		MAYBE_STATIC_ASSERT(*opt == 30);
	}

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt{in_place};

		EXPECT_TRUE(opt);
		EXPECT_EQ(opt->tot_default_ctor_calls, 1);
		EXPECT_EQ(opt->tot_instances, 1);
	}
	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt{in_place, 42};

		EXPECT_TRUE(opt);
		EXPECT_EQ(opt->_value, 42);
		EXPECT_EQ(opt->tot_int_ctor_calls, 1);
		EXPECT_EQ(opt->tot_instances, 1);
	}
	{
		static int tot_instances = 0;

		class SomeObject
		{
		public:
			bool _is_nullopt = false;
			int _x, _y, _z;

			SomeObject(nullopt_t) :
			    _is_nullopt(true)
			{
				tot_instances++;
			}
			SomeObject(int x, int y, int z) :
			    _x(x), _y(y), _z(z)
			{
				tot_instances++;
			}
		};

		{
			bwOptional<SomeObject> opt{in_place, nullopt};

			EXPECT_TRUE(opt);
			EXPECT_EQ(opt->_is_nullopt, true);
			EXPECT_EQ(tot_instances, 1);
		}
		{
			bwOptional<SomeObject> opt{in_place, 3, 1, 2};

			EXPECT_TRUE(opt);
			EXPECT_EQ(opt->_x, 3);
			EXPECT_EQ(opt->_y, 1);
			EXPECT_EQ(opt->_z, 2);
			EXPECT_EQ(tot_instances, 2);
		}
		{
			class ConstexprCtor
			{
			public:
				int _x, _y, _z;
				constexpr ConstexprCtor(int x, int y, int z) :
					_x(x), _y(y), _z(z)
				{}
			};
			constexpr bwOptional<ConstexprCtor> opt(in_place, 4, 7, 2);

			MAYBE_STATIC_ASSERT(opt);
			EXPECT_EQ(opt->_x, 4);
			EXPECT_EQ(opt->_y, 7);
			EXPECT_EQ(opt->_z, 2);
		}
	}
}

TEST(bwOptional, constructor_in_place_with_init_list)
{
	{
		bwOptional<std::vector<int>> opt(in_place, {42, 3, 15, 7});

		EXPECT_TRUE(opt);
		EXPECT_TRUE((*opt == std::vector<int>{42, 3, 15, 7}));
		EXPECT_EQ(opt->size(), 4);
	}
	{
		bwOptional<std::vector<int>> opt(in_place, {42, 3, 15, 7}, std::allocator<int>());

		EXPECT_TRUE(opt);
		EXPECT_TRUE((*opt == std::vector<int>{42, 3, 15, 7}));
		EXPECT_EQ(opt->size(), 4);
	}
	{
		class ConstexprInitListClass
		{
		public:
			int _x, _y;
			constexpr ConstexprInitListClass(std::initializer_list<int> ilist) :
			    _x(ilist.begin()[0]), _y(ilist.begin()[1])
			{}
		};
		constexpr bwOptional<ConstexprInitListClass> opt(in_place, {42, 15});

		{
			MAYBE_STATIC_ASSERT(opt);
			EXPECT_EQ(opt->_x, 42);
			EXPECT_EQ(opt->_y, 15);
		}
	}
	static_assert(!std::is_constructible<bwOptional<TestUtilClasses::NonTrivialConstructors>,
	                                     std::initializer_list<int>&>::value, "");
}

TEST(bwOptional, constructor_value_implicit)
{
	{
		bwOptional<int> opt{13};
		EXPECT_TRUE(opt);
		EXPECT_EQ(*opt, 13);
	}
	{
		constexpr bwOptional<int> opt(14);
		MAYBE_STATIC_ASSERT(opt);
		MAYBE_STATIC_ASSERT(*opt == 14);
	}
	{
		bwOptional<int> opt('f');
		EXPECT_TRUE(opt);
		EXPECT_EQ(*opt, static_cast<int>('f'));
	}
	{
		int value = 3;
		bwOptional<void * const> opt(&value);
		EXPECT_TRUE(opt);
		EXPECT_EQ(*opt, &value);
	}

	TestUtilClasses::NonTrivialConstructors::reset();
	{
		TestUtilClasses::NonTrivialConstructors data;
		bwOptional<TestUtilClasses::NonTrivialConstructors> opt(data);

		EXPECT_TRUE(opt);
		EXPECT_EQ(opt->tot_default_ctor_calls, 1);
		EXPECT_EQ(opt->tot_copy_ctor_calls, 1);
		EXPECT_EQ(opt->tot_instances, 2);
	}
}

TEST(bwOptional, constructor_value_explicit)
{
	TestUtilClasses::NonTrivialConstructors::reset();
	{
		bwOptional<TestUtilClasses::ExplicitConstructibleFromInt> opt(32);

		EXPECT_TRUE(opt);
		EXPECT_EQ(opt->_value, 32);
		EXPECT_EQ(opt->tot_int_ctor_calls, 1);
		EXPECT_EQ(opt->tot_instances, 1);
	}
	{
		constexpr bwOptional<TestUtilClasses::ConstexprConstructors> opt(2);

		MAYBE_STATIC_ASSERT(opt);
		MAYBE_STATIC_ASSERT(*opt == static_cast<TestUtilClasses::ConstexprConstructors>(2));
	}
}
