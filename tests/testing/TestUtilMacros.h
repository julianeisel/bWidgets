#pragma once

// If defined, use compile-time static_assert, else run-time EXPECT_TRUE.
#define USE_STATIC_ASSERT_IF_POSSIBLE

#ifdef USE_STATIC_ASSERT_IF_POSSIBLE
#  define MAYBE_STATIC_ASSERT(condition) static_assert((condition), "")
#else
#  define MAYBE_STATIC_ASSERT EXPECT_TRUE
#endif
