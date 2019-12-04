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

struct A {
};
struct B {
};

namespace std {

template<> struct hash<B> {
  size_t operator()(const B&)
  {
    return 0;
  }
};

}  // namespace std

template<typename To> struct ConvertibleToSimple {
  operator To() const
  {
    return To{};
  }
};

template<class To> struct ConvertibleTo {
  To to{};
  operator To&() &
  {
    return to;
  }
  operator To const&() const&
  {
    return to;
  }
  operator To &&() &&
  {
    return std::move(to);
  }
  operator To const &&() const&&
  {
    return std::move(to);
  }
};

template<typename HashExpr, typename Res = typename std::result_of<HashExpr>::type>
constexpr bool can_hash(int)
{
  return std::is_same<Res, size_t>::value;
}
template<typename> constexpr bool can_hash(long)
{
  return false;
}
template<typename T> constexpr bool can_hash()
{
  return can_hash<T>(0);
}

template<class Hash, class Key, class InputKey> void test_hash_enabled(InputKey const& key)
{
  static_assert(std::is_destructible<Hash>::value, "");
  // Enabled hash requirements
  static_assert(std::is_default_constructible<Hash>::value, "");
  static_assert(std::is_copy_constructible<Hash>::value, "");
  static_assert(std::is_move_constructible<Hash>::value, "");
  static_assert(std::is_copy_assignable<Hash>::value, "");
  static_assert(std::is_move_assignable<Hash>::value, "");

  // Hashable requirements
  static_assert(can_hash<Hash(Key&)>(), "");
  static_assert(can_hash<Hash(Key const&)>(), "");
  static_assert(can_hash<Hash(Key &&)>(), "");
  static_assert(can_hash<Hash const&(Key&)>(), "");
  static_assert(can_hash<Hash const&(Key const&)>(), "");
  static_assert(can_hash<Hash const&(Key &&)>(), "");

  static_assert(can_hash<Hash(ConvertibleToSimple<Key>&)>(), "");
  static_assert(can_hash<Hash(ConvertibleToSimple<Key> const&)>(), "");
  static_assert(can_hash<Hash(ConvertibleToSimple<Key> &&)>(), "");

  static_assert(can_hash<Hash(ConvertibleTo<Key>&)>(), "");
  static_assert(can_hash<Hash(ConvertibleTo<Key> const&)>(), "");
  static_assert(can_hash<Hash(ConvertibleTo<Key> &&)>(), "");
  static_assert(can_hash<Hash(ConvertibleTo<Key> const&&)>(), "");

  const Hash h{};
  EXPECT_EQ(h(key), h(key));
}
template<class T, class InputKey = T>
void test_hash_enabled_for_type(InputKey const& key = InputKey{})
{
  return test_hash_enabled<std::hash<T>, T, InputKey>(key);
}

template<class Hash, class Key> void test_hash_disabled()
{
  // Disabled hash requirements
  static_assert(!std::is_default_constructible<Hash>::value, "");
  static_assert(!std::is_copy_constructible<Hash>::value, "");
  static_assert(!std::is_move_constructible<Hash>::value, "");
  static_assert(!std::is_copy_assignable<Hash>::value, "");
  static_assert(!std::is_move_assignable<Hash>::value, "");

  static_assert(
      !std::is_function<
          typename std::remove_pointer<typename std::remove_reference<Hash>::type>::type>::value,
      "");

  // Hashable requirements
  static_assert(!can_hash<Hash(Key&)>(), "");
  static_assert(!can_hash<Hash(Key const&)>(), "");
  static_assert(!can_hash<Hash(Key &&)>(), "");
  static_assert(!can_hash<Hash const&(Key&)>(), "");
  static_assert(!can_hash<Hash const&(Key const&)>(), "");
  static_assert(!can_hash<Hash const&(Key &&)>(), "");

  static_assert(!can_hash<Hash(ConvertibleToSimple<Key>&)>(), "");
  static_assert(!can_hash<Hash(ConvertibleToSimple<Key> const&)>(), "");
  static_assert(!can_hash<Hash(ConvertibleToSimple<Key> &&)>(), "");

  static_assert(!can_hash<Hash(ConvertibleTo<Key>&)>(), "");
  static_assert(!can_hash<Hash(ConvertibleTo<Key> const&)>(), "");
  static_assert(!can_hash<Hash(ConvertibleTo<Key> &&)>(), "");
  static_assert(!can_hash<Hash(ConvertibleTo<Key> const&&)>(), "");
}
template<class T> void test_hash_disabled_for_type()
{
  return test_hash_disabled<std::hash<T>, T>();
}

TEST(bwOptional, hash)
{
  const std::size_t nullopt_hash = std::hash<bwOptional<double>>{}(bwOptional<double>{});

  {
    bwOptional<int> opt;
    EXPECT_EQ(std::hash<bwOptional<int>>{}(opt), nullopt_hash);
    opt = 2;
    EXPECT_EQ(std::hash<bwOptional<int>>{}(opt), std::hash<int>{}(*opt));
  }
  {
    bwOptional<std::string> opt;
    EXPECT_EQ(std::hash<bwOptional<std::string>>{}(opt), nullopt_hash);
    opt = std::string("123");
    EXPECT_EQ(std::hash<bwOptional<std::string>>{}(opt), std::hash<std::string>{}(*opt));
  }
  {
    bwOptional<std::unique_ptr<int>> opt;
    EXPECT_EQ(std::hash<bwOptional<std::unique_ptr<int>>>{}(opt), nullopt_hash);
    opt = std::unique_ptr<int>(new int(3));
    EXPECT_EQ(std::hash<bwOptional<std::unique_ptr<int>>>{}(opt),
              std::hash<std::unique_ptr<int>>{}(*opt));
  }
  {
    test_hash_enabled_for_type<bwOptional<int>>();
    test_hash_enabled_for_type<bwOptional<int*>>();
    test_hash_enabled_for_type<bwOptional<const int>>();
    test_hash_enabled_for_type<bwOptional<int* const>>();

    test_hash_disabled_for_type<bwOptional<A>>();
    test_hash_disabled_for_type<bwOptional<const A>>();

    test_hash_enabled_for_type<bwOptional<B>>();
    test_hash_enabled_for_type<bwOptional<const B>>();
  }
}
