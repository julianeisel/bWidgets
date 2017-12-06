#pragma once

#include <memory>


namespace bWidgets {

/**
 * Portable version of __PRETTY_FUNCTION__.
 */
#ifdef _MSC_VER
#  define PRETTY_FUNCTION __FUNCSIG__
#else
#  define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

/**
 * \brief Alias for `std::unique_ptr()`.
 *
 * With this,`std::unique_ptr()` follows our naming conventions.
 * In future this may also be useful in case we add our own unique-pointer
 * implementation.
 *
 * We currently don't need a `std::shared_ptr()` version of this, and would
 * like to avoid it.
 */
template<typename _PointerType>
using bwPointer = std::unique_ptr<_PointerType>;

/**
 * \brief Own implementation of `std::make_unique()` for C++11.
 *
 * `std::make_unique()` is only introduced in C++14, this is an
 * implementation that matches its behavior. It also follows bWidgets'
 * naming conventions.
 *
 * E.g. this:
 * \code
 * bwPointer<SomeType> type = bwPointer<SomeType>(new SomeType(some_arg));
 * \endcode
 * shortens to:
 * \code
 * bwPointer<SomeType> type = bwPointer_new<SomeType>(some_arg);
 * \endcode
 *
 * Since the type is clear from passing it to `bwPointer_new()` as template
 * parameter, it's fine to use `auto` too:
 * \code
 * auto type = bwPointer_new<SomeType>(some_arg);
 * \endcode
 */
template<typename _PointerType, typename... _Args>
inline bwPointer<_PointerType> bwPointer_new(_Args&&... args)
{
	return bwPointer<_PointerType>(new _PointerType(std::forward<_Args>(args)...));
}

/**
 * \overload
 * Technically not needed since overloaded version also accepts no arguments.
 * Doing so confuses some IDE's however, which this overload fixes.
 */
template<typename _PointerType>
inline bwPointer<_PointerType> bwPointer_new()
{
	return bwPointer<_PointerType>(new _PointerType());
}

} // namespace bWidgets

/**
 * \brief Extension of the std library.
 */
namespace std {

/**
 * Helper to ensure #make_unique() implementation below only causes an error
 * when it's actually called. `static_assert(false)` would fail at template
 * definition time, `static_assert(always_false<T>::value)` only fails at
 * template instantiation time.
 */
template<typename... T>
struct always_false
{
	static constexpr bool value = false;
};

/**
 * Make calling C++14's `std::make_unique()` fail with an error message hinting
 * to bWidgets' \link bWidgets::bwPointer_new bwPointer_new().
 */
template<typename _PointerType, typename... _Args>
inline constexpr std::unique_ptr<_PointerType> make_unique(_Args&&...)
{
	static_assert(
	            always_false<_Args...>::value,
	            "Error: `std::make_unique()` is not available in C++11 and shouldn't be "
	            "used! Use bWidgets' own implementation `bwPointer_new()` instead.");
	return{};
}

}
