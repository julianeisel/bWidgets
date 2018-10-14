#include "gtest/gtest.h"

#include "TestUtilClasses.h"

// ---- Init static util vars ----

int TestUtilClasses::NonTrivialConstructors::tot_default_ctor_calls = 0;
int TestUtilClasses::NonTrivialConstructors::tot_int_ctor_calls = 0;
int TestUtilClasses::NonTrivialConstructors::tot_copy_ctor_calls = 0;
int TestUtilClasses::NonTrivialConstructors::tot_move_ctor_calls = 0;
int TestUtilClasses::NonTrivialConstructors::tot_dtor_calls = 0;
int TestUtilClasses::NonTrivialConstructors::tot_instances = 0;

int TestUtilClasses::CustomSwap::tot_int_ctor_calls = 0;
int TestUtilClasses::CustomSwap::tot_move_ctor_calls = 0;
int TestUtilClasses::CustomSwap::tot_dtor_calls = 0;
int TestUtilClasses::CustomSwap::tot_swap_calls = 0;

// --------


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
