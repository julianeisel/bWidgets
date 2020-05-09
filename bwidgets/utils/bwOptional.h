/*
 * Much of this is based on Clang's implementation of std::optional. For fair
 * credit and copyright compliance, the following notice additionally applies:
 *
 * Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT
 */

#pragma once

#include <cassert>
#include <functional>
#include <type_traits>

/**
 * \class bWidgets::bwOptional
 * \brief Own implementation of `std::optional` for C++11.
 *
 * This implementation should follow the C++17 standard of `std::optional` as
 * far as possible with C++11. It can not be guaranteed to follow the standard
 * precisely, however if inconsistencies are found, please report them.
 *
 * For information on how to use bwOptional, please refer to the existing
 * documentation of `std::optional`.
 *
 * ----
 *
 * Following the standard makes for a quite complicated, scary looking
 * implementation. In return it should be safe and have zero cost, while
 * actually being rather intuitive to use.
 *
 * Known standard violations:
 * * Deduction guides missing (deduction guides are only introduced in C++17).
 * * `constexpr` is disabled for some functions as support for it is limited in
 *    C++11.
 * * Exception safety is ignored (no `noexcept`), just like in the rest of
 *   bWidgets.
 * * `std::swap(bwOptional<T>&, bwOptional<T>&)` should be excluded from
 *   overload resolution unless `std::is_swappable_v<T>` is `true`. The latter
 *   one is a C++17 trait though, which we'd have to implement on our own if
 *   needed.
 */

namespace bWidgets {

/**
 * \brief Exception for access of disengaged #bwOptional.
 */
class bad_optional_access : public std::exception {
 public:
  const char* what() const noexcept override
  {
    return "bad optional access";
  }
};

/**
 * \brief Empty helper class to indicate disengaged #bwOptional.
 */
struct nullopt_t {
  explicit constexpr nullopt_t(int)
  {
  }
};
constexpr nullopt_t nullopt{42};

/**
 * \brief Empty helper class to tag #bwOptional constructors to use in-place
 *        construction.
 *
 * In-place construction means the arguments passed to the constructor together
 * with #in_place, will be forwarded (as in, perfect forwarding) to the
 * constructor of the type `T` of a `bwOptional<T>`.
 *
 * More concrete, initializing a #bwOptional like this:
 * \code{.cc}
 * bwOptional<SomeType> opt(in_place, arg1, std::move(arg2), 10.0f);
 * \endcode
 * will construct an instance of `SomeType` as if using:
 * \code{.cc}
 * SomeType some_type_var(arg1, std::move(arg2), 10.0f);
 * \endcode
 */
struct in_place_t {
  explicit in_place_t() = default;
};
constexpr in_place_t in_place{};

}  // namespace bWidgets

#include "internal/bwOptional_internal.h"

namespace bWidgets {

using namespace bwOptional_internal;

template<typename _T>
class bwOptional : private bwOptional_Base<_T>,
                   private bwOptional_DeleteCopyCtorIfNeeded<_T>,
                   private bwOptional_DeleteMoveCtorIfNeeded<_T>,
                   private bwOptional_DeleteCopyAssignOpIfNeeded<_T>,
                   private bwOptional_DeleteMoveAssignOpIfNeeded<_T> {
 private:
  static_assert(!std::is_reference<_T>::value,
                "Instantiating a bwOptional with a reference type is not allowed.");
  static_assert(std::is_destructible<_T>::value,
                "Instantiating a bwOptional with a non-destructible type is not allowed.");
  static_assert(!std::is_same<typename std::remove_cv<_T>::type, nullopt_t>::value,
                "Instantiating a bwOptional with nullopt_t is not allowed.");
  static_assert(!std::is_same<typename std::remove_cv<_T>::type, in_place_t>::value,
                "Instantiating a bwOptional with in_place_t is not allowed.");

  using Base = bwOptional_Base<_T>;

 public:
  using value_type = _T;

  constexpr bwOptional()
  {
  }
  constexpr bwOptional(nullopt_t)
  {
  }
  // Copy constructor.
  constexpr bwOptional(const bwOptional&) = default;
  // Move constructor.
  constexpr bwOptional(bwOptional&&) = default;

  // Converting copy constructors.
  template<
      typename _U,
      typename std::enable_if<
          !std::is_same<_T, _U>::value && std::is_constructible<_T, const _U&>::value &&
              std::is_convertible<const _U&, _T>::value && !constructible_from_opt<_T, _U>::value,
          bool>::type = false>
  bwOptional(const bwOptional<_U>& other)
  {
    if (other) {
      emplace(*other);
    }
  }
  template<
      typename _U,
      typename std::enable_if<
          !std::is_same<_T, _U>::value && std::is_constructible<_T, const _U&>::value &&
              !std::is_convertible<const _U&, _T>::value && !constructible_from_opt<_T, _U>::value,
          bool>::type = false>
  explicit bwOptional(const bwOptional<_U>& other)
  {
    if (other) {
      emplace(*other);
    }
  }

  // Converting move constructors.
  template<typename _U,
           typename std::enable_if<
               !std::is_same<_T, _U>::value && std::is_constructible<_T, _U&&>::value &&
                   std::is_convertible<_U&&, _T>::value && !constructible_from_opt<_T, _U>::value,
               bool>::type = false>
  bwOptional(bwOptional<_U>&& other)
  {
    if (other) {
      emplace(std::move(*other));
    }
  }
  template<typename _U,
           typename std::enable_if<
               !std::is_same<_T, _U>::value && std::is_constructible<_T, _U&&>::value &&
                   !std::is_convertible<_U&&, _T>::value && !constructible_from_opt<_T, _U>::value,
               bool>::type = false>
  explicit bwOptional(bwOptional<_U>&& other)
  {
    if (other) {
      emplace(std::move(*other));
    }
  }

  template<typename... Args,
           typename std::enable_if<std::is_constructible<_T, Args...>::value, bool>::type = false>
  constexpr explicit bwOptional(in_place_t, Args&&... args)
      : Base(in_place, std::forward<Args>(args)...)
  {
  }

  template<typename _U,
           typename... Args,
           typename std::enable_if<
               std::is_constructible<_T, std::initializer_list<_U>&, Args&&...>::value,
               bool>::type = false>
  constexpr explicit bwOptional(in_place_t, std::initializer_list<_U> ilist, Args&&... args)
      : Base(in_place, ilist, std::forward<Args>(args)...)
  {
  }

  template<typename _U = _T,
           typename std::enable_if<std::is_constructible<_T, _U&&>::value &&
                                       !std::is_same<std::decay<_U>, in_place_t>::value &&
                                       !std::is_same<std::decay<_U>, bwOptional<_T>>::value &&
                                       std::is_convertible<_U&&, _T>::value,
                                   bool>::type = false>
  constexpr bwOptional(_U&& value) : Base(in_place, std::forward<_U>(value))
  {
  }
  template<typename _U = _T,
           typename std::enable_if<std::is_constructible<_T, _U&&>::value &&
                                       !std::is_same<std::decay<_U>, in_place_t>::value &&
                                       !std::is_same<std::decay<_U>, bwOptional<_T>>::value &&
                                       !std::is_convertible<_U&&, _T>::value,
                                   bool>::type = false>
  constexpr explicit bwOptional(_U&& value) : Base(in_place, std::forward<_U>(value))
  {
  }

  /* Destructor must be implicitly defined! That way this optional may be
   * trivially destructible, which the standard requires if _T is trivially
   * destructible. A base class (bwOptional_Storage) can then conditionally
   * make the destructor trivial. */
  ~bwOptional() = default;

  bwOptional& operator=(nullopt_t)
  {
    reset();
    return *this;
  }
  bwOptional& operator=(const bwOptional& other) = default;
  bwOptional& operator=(bwOptional&&) = default;

  template<typename _U = _T,
           typename std::enable_if<
               !std::is_same<std::decay<_U>, bwOptional<_T>>::value &&
                   std::is_constructible<_T, _U>::value && std::is_assignable<_T&, _U>::value &&
                   (!std::is_scalar<_T>::value || !std::is_same<std::decay<_U>, _T>::value),
               bool>::type = false>
  bwOptional& operator=(_U&& value)
  {
    if (this->has_value()) {
      this->get() = std::forward<_U>(value);
    }
    else {
      this->construct(std::forward<_U>(value));
    }
    return *this;
  }

  template<
      typename _U,
      typename std::enable_if<
          !std::is_same<_T, _U>::value && std::is_constructible<_T, const _U&>::value &&
              std::is_assignable<_T&, const _U&>::value &&
              !constructible_from_opt<_T, _U>::value && !assignable_from_optional<_T, _U>::value,
          bool>::type = false>
  bwOptional& operator=(const bwOptional<_U>& other)
  {
    this->assign_from(other);
    return *this;
  }

  template<typename _U,
           typename std::enable_if<
               !std::is_same<_T, _U>::value && std::is_constructible<_T, _U>::value &&
                   std::is_assignable<_T&, _U>::value && !constructible_from_opt<_T, _U>::value &&
                   !assignable_from_optional<_T, _U>::value,
               bool>::type = false>
  bwOptional& operator=(bwOptional<_U>&& other)
  {
    this->assign_from(std::move(other));
    return *this;
  }

  /*constexpr*/  // While this function itself could be constexpr, it calls addressof which is not
                 // constexpr in C++11.
  const _T* operator->() const
  {
    assert(has_value() && "attempt to get pointer to the value of a disengaged optional");
    return std::addressof(this->get());
  }
  /*constexpr*/ _T* operator->()
  {
    assert(has_value() && "attempt to get pointer to the value of a disengaged optional");
    return std::addressof(this->get());
  }

  constexpr const _T& operator*() const&
  {
    // Need to squeeze this into a single instruction to workaround bad C++11 constexpr support.
    return (assert(has_value() && "attempt to get value of a disengaged optional"), this->get());
  }
  /*constexpr*/ _T& operator*() &
  {
    // Need to squeeze this into a single instruction to workaround bad C++11 constexpr support.
    return (assert(has_value() && "attempt to get value of a disengaged optional"), this->get());
  }
  constexpr const _T&& operator*() const&&
  {
    // Need to squeeze this into a single instruction to workaround bad C++11 constexpr support.
    return (assert(has_value() && "attempt to get value of a disengaged optional"),
            std::move(this->get()));
  }
  /*constexpr*/ _T&& operator*() &&
  {
    return (assert(has_value() && "attempt to get value of a disengaged optional"),
            std::move(this->get()));
  }

  constexpr explicit operator bool() const
  {
    return this->has_value();
  }

  using Base::get;  // XXX shouldn't be exposed publicly, but needed for assign_from...
  using Base::has_value;

  /*constexpr*/ _T& value() &
  {
    if (!has_value()) {
      throw bad_optional_access();
    }
    return this->get();
  }
  constexpr const _T& value() const&
  {
    // Need to squeeze this into a single instruction to workaround bad C++11 constexpr support.
    return (!has_value() ? throw bad_optional_access() : (void)0, this->get());
  }
  /*constexpr*/ _T&& value() &&
  {
    if (!has_value()) {
      throw bad_optional_access();
    }
    return std::move(this->get());
  }
  constexpr const _T&& value() const&&
  {
    // Need to squeeze this into a single instruction to workaround bad C++11 constexpr support.
    return (!has_value() ? throw bad_optional_access() : (void)0, std::move(this->get()));
  }

  template<typename _U> constexpr _T value_or(_U&& or_value) const&
  {
    static_assert(std::is_copy_constructible<_T>::value, "");
    static_assert(std::is_convertible<_U&&, _T>::value, "");
    return has_value() ? **this : static_cast<_T>(std::forward<_U>(or_value));
  }
  template<typename _U>
  /*constexpr*/ _T value_or(_U&& or_value) &&
  {
    static_assert(std::is_move_constructible<_T>::value, "");
    static_assert(std::is_convertible<_U&&, _T>::value, "");
    return has_value() ? std::move(**this) : static_cast<_T>(std::forward<_U>(or_value));
  }

  void swap(bwOptional& other)
  {
    if (this->has_value() && other.has_value()) {
      using std::swap;
      swap(this->get(), other.get());
    }
    else if (other.has_value()) {
      this->construct(std::move(other.get()));
      other.reset();
    }
    else if (this->has_value()) {
      other.construct(std::move(this->get()));
      this->reset();
    }
  }

  using Base::reset;

  template<
      typename... Args,
      typename std::enable_if<std::is_constructible<_T, Args&&...>::value, bool>::type = false>
  _T& emplace(Args&&... args)
  {
    this->reset();
    this->construct(std::forward<Args>(args)...);
    return this->get();
  }
  template<typename _U,
           typename... Args,
           typename std::enable_if<
               std::is_constructible<_T, std::initializer_list<_U>&, Args&&...>::value,
               bool>::type = false>
  _T& emplace(std::initializer_list<_U> ilist, Args&&... args)
  {
    this->reset();
    this->construct(ilist, std::forward<Args>(args)...);
    return this->get();
  }
};

template<typename _T, typename _U>
constexpr bool operator==(const bwOptional<_T>& lhs, const bwOptional<_U>& rhs)
{
  return (bool(lhs) != bool(rhs)) ? false : (!bool(lhs) ? true : (*lhs == *rhs));
}
template<typename _T> constexpr bool operator==(const bwOptional<_T>& opt, nullopt_t)
{
  return !opt;
}
template<typename _T> constexpr bool operator==(nullopt_t, const bwOptional<_T>& opt)
{
  return !opt;
}
template<typename _T, typename _U>
constexpr bool operator==(const bwOptional<_T>& opt, const _U& value)
{
  return bool(opt) ? *opt == value : false;
}
template<typename _T, typename _U>
constexpr bool operator==(const _T& value, const bwOptional<_U>& opt)
{
  return bool(opt) ? value == *opt : false;
}

template<typename _T, typename _U>
constexpr bool operator!=(const bwOptional<_T>& lhs, const bwOptional<_U>& rhs)
{
  return (bool(lhs) != bool(rhs)) ? true : (!bool(lhs) ? false : (*lhs != *rhs));
}
template<typename _T> constexpr bool operator!=(const bwOptional<_T>& opt, nullopt_t)
{
  return bool(opt);
}
template<typename _T> constexpr bool operator!=(nullopt_t, const bwOptional<_T>& opt)
{
  return bool(opt);
}
template<typename _T, typename _U>
constexpr bool operator!=(const bwOptional<_T>& opt, const _U& value)
{
  return bool(opt) ? *opt != value : true;
}
template<typename _T, typename _U>
constexpr bool operator!=(const _T& value, const bwOptional<_U>& opt)
{
  return bool(opt) ? value != *opt : true;
}

template<typename _T, typename _U>
constexpr bool operator<(const bwOptional<_T>& lhs, const bwOptional<_U>& rhs)
{
  return !bool(rhs) ? false : (!bool(lhs) ? true : (*lhs < *rhs));
}
template<typename _T> constexpr bool operator<(const bwOptional<_T>& opt, nullopt_t)
{
  return false;
}
template<typename _T> constexpr bool operator<(nullopt_t, const bwOptional<_T>& opt)
{
  return bool(opt);
}
template<typename _T, typename _U>
constexpr bool operator<(const bwOptional<_T>& opt, const _U& value)
{
  return bool(opt) ? *opt < value : true;
}
template<typename _T, typename _U>
constexpr bool operator<(const _T& value, const bwOptional<_U>& opt)
{
  return bool(opt) ? value < *opt : false;
}

template<typename _T, typename _U>
constexpr bool operator<=(const bwOptional<_T>& lhs, const bwOptional<_U>& rhs)
{
  return !bool(lhs) ? true : (!bool(rhs) ? false : (*lhs <= *rhs));
}
template<typename _T> constexpr bool operator<=(const bwOptional<_T>& opt, nullopt_t)
{
  return !opt;
}
template<typename _T> constexpr bool operator<=(nullopt_t, const bwOptional<_T>& opt)
{
  return true;
}
template<typename _T, typename _U>
constexpr bool operator<=(const bwOptional<_T>& opt, const _U& value)
{
  return bool(opt) ? *opt <= value : true;
}
template<typename _T, typename _U>
constexpr bool operator<=(const _T& value, const bwOptional<_U>& opt)
{
  return bool(opt) ? value <= *opt : false;
}

template<typename _T, typename _U>
constexpr bool operator>(const bwOptional<_T>& lhs, const bwOptional<_U>& rhs)
{
  return !bool(lhs) ? false : (!bool(rhs) ? true : (*lhs > *rhs));
}
template<typename _T> constexpr bool operator>(const bwOptional<_T>& opt, nullopt_t)
{
  return bool(opt);
}
template<typename _T> constexpr bool operator>(nullopt_t, const bwOptional<_T>& opt)
{
  return false;
}
template<typename _T, typename _U>
constexpr bool operator>(const bwOptional<_T>& opt, const _U& value)
{
  return bool(opt) ? *opt > value : false;
}
template<typename _T, typename _U>
constexpr bool operator>(const _T& value, const bwOptional<_U>& opt)
{
  return bool(opt) ? value > *opt : true;
}

template<typename _T, typename _U>
constexpr bool operator>=(const bwOptional<_T>& lhs, const bwOptional<_U>& rhs)
{
  return !bool(rhs) ? true : (!bool(lhs) ? false : (*lhs >= *rhs));
}
template<typename _T> constexpr bool operator>=(const bwOptional<_T>& opt, nullopt_t)
{
  return true;
}
template<typename _T> constexpr bool operator>=(nullopt_t, const bwOptional<_T>& opt)
{
  return !opt;
}
template<typename _T, typename _U>
constexpr bool operator>=(const bwOptional<_T>& opt, const _U& value)
{
  return bool(opt) ? *opt >= value : false;
}
template<typename _T, typename _U>
constexpr bool operator>=(const _T& value, const bwOptional<_U>& opt)
{
  return bool(opt) ? value >= *opt : true;
}

template<typename _T> constexpr bwOptional<typename std::decay<_T>::type> make_optional(_T&& value)
{
  return bwOptional<typename std::decay<_T>::type>(std::forward<_T>(value));
}
template<typename _T, typename... Args> constexpr bwOptional<_T> make_optional(Args&&... args)
{
  return bwOptional<_T>(in_place, std::forward<Args>(args)...);
}
template<typename _T, typename _U, typename... Args>
constexpr bwOptional<_T> make_optional(std::initializer_list<_U> ilist, Args&&... args)
{
  return bwOptional<_T>(in_place, ilist, std::forward<Args>(args)...);
}

template<typename _T,
         typename std::enable_if<std::is_move_constructible<_T>::value
                                 // is_swappable is C++17 only. We could implement it if really
                                 // needed...
                                 //             , std::is_swappable<_T>::value
                                 ,
                                 bool>::type = false>
void swap(bwOptional<_T>& lhs, bwOptional<_T>& rhs)
{
  lhs.swap(rhs);
}

}  // namespace bWidgets

/** \cond HIDDEN_SYMBOLS */  // Start excluding symbols from doxygen input.

namespace std {

/* This specialization for std::hash<bwOptional<>> only seems to work on Linux GCC. */
#if defined(__GNUC__) && !defined(__clang__)
template<typename _Type, typename> using __enable_hash_helper_impl = _Type;
template<typename _Type, typename _KeyType>
using __enable_hash_helper =
    __enable_hash_helper_impl<_Type, decltype(hash<_KeyType>()(declval<_KeyType>()))>;

template<typename _T>
struct hash<__enable_hash_helper<bWidgets::bwOptional<_T>, typename remove_const<_T>::type>> {
  using argument_type = bWidgets::bwOptional<_T>;
  using result_type = size_t;

  result_type operator()(const argument_type& opt) const
  {
    return opt ? hash<typename remove_const<_T>::type>()(*opt) : 0;
  }
};
#endif

/** \endcond */  // End excluding symbols from Doxygen input.

}  // namespace std
