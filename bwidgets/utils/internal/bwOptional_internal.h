/*
 * Much of this is based on Clang's implementation of std::optional. For fair
 * credit and copyright compliance, the following notice additionally applies:
 *
 * Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT (parent
 * directory)
 */

#pragma once

#include <type_traits>

/** \cond HIDDEN_SYMBOLS */  // Excludes the following from doxygen input.

namespace bWidgets {
template<typename _T> class bwOptional;

namespace bwOptional_internal {

template<typename _T,
         typename _U,
         bool = std::is_constructible<_T, const bwOptional<_U>&>::value&&
             std::is_constructible<_T, bwOptional<_U>&>::value&&
                 std::is_constructible<_T, const bwOptional<_U>&&>::value&&
                     std::is_constructible<_T, bwOptional<_U>&&>::value&&
                         std::is_convertible<const bwOptional<_U>&, _T>::value&&
                             std::is_convertible<bwOptional<_U>&, _T>::value&&
                                 std::is_convertible<const bwOptional<_U>&&, _T>::value&&
                                     std::is_convertible<bwOptional<_U>&&, _T>::value>
struct constructible_from_opt : public std::true_type {
};
template<typename _T, typename _U>
struct constructible_from_opt<_T, _U, false> : public std::false_type {
};

template<typename _T,
         typename _U,
         bool = std::is_assignable<_T&, const bwOptional<_U>&>::value&&
             std::is_assignable<_T&, bwOptional<_U>&>::value&&
                 std::is_assignable<_T&, const bwOptional<_U>&&>::value&&
                     std::is_assignable<_T&, bwOptional<_U>&&>::value>
struct assignable_from_optional : public std::true_type {
};
template<typename _T, typename _U>
struct assignable_from_optional<_T, _U, false> : public std::false_type {
};

/**
 * Storage for optionals with a trivially destructible type _T. In that case,
 * bwOptional (and therefore its base classes) is supposed to be trivially
 * destructible too, meaning the destructor has to be implicitly defined.
 */
template<typename _T, bool = std::is_trivially_destructible<_T>::value> struct bwOptional_Storage {
 public:
  constexpr bwOptional_Storage() : _empty_value(), _is_valid(false)
  {
  }
  template<typename... Args>
  constexpr explicit bwOptional_Storage(in_place_t, Args&&... args)
      : _value(std::forward<Args>(args)...), _is_valid(true)
  {
  }
  constexpr bwOptional_Storage(const bwOptional_Storage&) = default;
  constexpr bwOptional_Storage(bwOptional_Storage&&) = default;

  /* Trivial destructor */
  ~bwOptional_Storage() = default;

  void reset()
  {
    if (_is_valid) {
      _is_valid = false;
    }
  }

  union {
    char _empty_value;
    _T _value;
  };
  bool _is_valid;
};
template<typename _T> class bwOptional_Storage<_T, false> {
 public:
  constexpr bwOptional_Storage() : _empty_value(), _is_valid(false)
  {
  }
  template<typename... Args>
  constexpr explicit bwOptional_Storage(in_place_t, Args&&... args)
      : _value(std::forward<Args>(args)...), _is_valid(true)
  {
  }
  constexpr bwOptional_Storage(const bwOptional_Storage&) = default;
  constexpr bwOptional_Storage(bwOptional_Storage&&) = default;

  ~bwOptional_Storage()
  {
    if (_is_valid) {
      _value.~_T();
    }
  }

  void reset()
  {
    if (_is_valid) {
      _value.~_T();
      _is_valid = false;
    }
  }

  union {
    char _empty_value;
    _T _value;
  };
  bool _is_valid;
};

template<typename _T> class bwOptional_StorageAccessor : private bwOptional_Storage<_T> {
 public:
  using bwOptional_Storage<_T>::bwOptional_Storage;

  template<typename... Args> void construct(Args&&... args)
  {
    assert(!has_value());
    // Placement new, constructs the object at given address. Cast to void* is to cast away
    // possible const.
    ::new ((void*)std::addressof(this->_value)) _T(std::forward<Args>(args)...);
    this->_is_valid = true;
  }
  template<typename _U> void construct_from(_U&& from)
  {
    if (from.has_value()) {
      construct(std::forward<_U>(from).get());
    }
  }
  template<typename _U> void assign_from(_U&& from)
  {
    if (this->has_value() && from.has_value()) {
      this->_value = std::forward<_U>(from).get();
    }
    else if (from.has_value()) {
      construct(std::forward<_U>(from).get());
    }
    else if (this->has_value()) {
      this->reset();
    }
  }

  using bwOptional_Storage<_T>::reset;

  constexpr bool has_value() const
  {
    return this->_is_valid;
  }

  /*constexpr*/ _T& get() &
  {
    return this->_value;
  }
  constexpr const _T& get() const&
  {
    return this->_value;
  }
  /*constexpr*/ _T&& get() &&
  {
    return std::move(this->_value);
  }
  constexpr const _T&& get() const&&
  {
    return std::move(this->_value);
  }
};

template<typename _T, bool = std::is_trivially_copy_constructible<_T>::value>
struct bwOptional_Base_AddCopyCtor : public bwOptional_StorageAccessor<_T> {
  using bwOptional_StorageAccessor<_T>::bwOptional_StorageAccessor;
};
template<typename _T>
struct bwOptional_Base_AddCopyCtor<_T, false> : public bwOptional_StorageAccessor<_T> {
  using bwOptional_StorageAccessor<_T>::bwOptional_StorageAccessor;

  bwOptional_Base_AddCopyCtor() = default;
  bwOptional_Base_AddCopyCtor(const bwOptional_Base_AddCopyCtor& other)
  {
    this->construct_from(other);
  }
  bwOptional_Base_AddCopyCtor(bwOptional_Base_AddCopyCtor&&) = default;
};

template<typename _T, bool = std::is_trivially_move_constructible<_T>::value>
struct bwOptional_Base_AddMoveAndCopyCtors : public bwOptional_Base_AddCopyCtor<_T> {
  using bwOptional_Base_AddCopyCtor<_T>::bwOptional_Base_AddCopyCtor;
};
template<typename _T>
struct bwOptional_Base_AddMoveAndCopyCtors<_T, false> : public bwOptional_Base_AddCopyCtor<_T> {
  using bwOptional_Base_AddCopyCtor<_T>::bwOptional_Base_AddCopyCtor;

  bwOptional_Base_AddMoveAndCopyCtors() = default;
  bwOptional_Base_AddMoveAndCopyCtors(const bwOptional_Base_AddMoveAndCopyCtors&) = default;
  bwOptional_Base_AddMoveAndCopyCtors(bwOptional_Base_AddMoveAndCopyCtors&& other)
  {
    this->construct_from(std::move(other));
  }
};

template<typename _T>
struct bwOptional_Base_AddCtorsAndCopyAssignOp : public bwOptional_Base_AddMoveAndCopyCtors<_T> {
  using bwOptional_Base_AddMoveAndCopyCtors<_T>::bwOptional_Base_AddMoveAndCopyCtors;
};

template<typename _T> class bwOptional_Base : public bwOptional_Base_AddCtorsAndCopyAssignOp<_T> {
 public:
  using bwOptional_Base_AddCtorsAndCopyAssignOp<_T>::bwOptional_Base_AddCtorsAndCopyAssignOp;

  constexpr bwOptional_Base() = default;
  constexpr bwOptional_Base(const bwOptional_Base&) = default;
  constexpr bwOptional_Base(bwOptional_Base&&) = default;

  /* Implicitly defined so bwOptional_Storage can decide to make this
   * optional trivially destructible or not. */
  ~bwOptional_Base() = default;

  bwOptional_Base& operator=(const bwOptional_Base& other)
  {
    this->assign_from(other);
    return *this;
  }
  bwOptional_Base& operator=(bwOptional_Base&& other)
  {
    this->assign_from(std::move(other));
    return *this;
  }
  template<
      typename _U,
      typename std::enable_if<
          !std::is_same<_T, _U>::value && std::is_constructible<_T, const _U&>::value &&
              std::is_assignable<_T&, const _U&>::value &&
              !constructible_from_opt<_T, _U>::value && !assignable_from_optional<_T, _U>::value,
          bool>::type = false>
  bwOptional_Base& operator=(const bwOptional_Base<_U>& other)
  {
    this->assign_from(other);
    return *this;
  }
};

template<typename _T, bool = std::is_copy_constructible<_T>::value>
struct bwOptional_DeleteCopyCtorIfNeeded {
};
template<typename _T> struct bwOptional_DeleteCopyCtorIfNeeded<_T, false> {
  bwOptional_DeleteCopyCtorIfNeeded() = default;
  bwOptional_DeleteCopyCtorIfNeeded(const bwOptional_DeleteCopyCtorIfNeeded&) = delete;
  bwOptional_DeleteCopyCtorIfNeeded(bwOptional_DeleteCopyCtorIfNeeded&&) = default;
};

template<typename _T, bool = std::is_move_constructible<_T>::value>
struct bwOptional_DeleteMoveCtorIfNeeded {
};
template<typename _T> struct bwOptional_DeleteMoveCtorIfNeeded<_T, false> {
  bwOptional_DeleteMoveCtorIfNeeded() = default;
  bwOptional_DeleteMoveCtorIfNeeded(const bwOptional_DeleteMoveCtorIfNeeded&) = default;
  bwOptional_DeleteMoveCtorIfNeeded(bwOptional_DeleteMoveCtorIfNeeded&&) = delete;
};

template<typename _T,
         bool = std::is_copy_constructible<_T>::value&& std::is_copy_assignable<_T>::value>
struct bwOptional_DeleteCopyAssignOpIfNeeded {
};
template<typename _T> struct bwOptional_DeleteCopyAssignOpIfNeeded<_T, false> {
  bwOptional_DeleteCopyAssignOpIfNeeded() = default;
  bwOptional_DeleteCopyAssignOpIfNeeded(const bwOptional_DeleteCopyAssignOpIfNeeded&) = default;
  bwOptional_DeleteCopyAssignOpIfNeeded(bwOptional_DeleteCopyAssignOpIfNeeded&&) = default;
  bwOptional_DeleteCopyAssignOpIfNeeded& operator=(const bwOptional_DeleteCopyAssignOpIfNeeded&) =
      delete;
  bwOptional_DeleteCopyAssignOpIfNeeded& operator=(bwOptional_DeleteCopyAssignOpIfNeeded&&) =
      default;
};

template<typename _T,
         bool = std::is_move_constructible<_T>::value&& std::is_move_assignable<_T>::value>
struct bwOptional_DeleteMoveAssignOpIfNeeded {
};
template<typename _T> struct bwOptional_DeleteMoveAssignOpIfNeeded<_T, false> {
  bwOptional_DeleteMoveAssignOpIfNeeded() = default;
  bwOptional_DeleteMoveAssignOpIfNeeded(const bwOptional_DeleteMoveAssignOpIfNeeded&) = default;
  bwOptional_DeleteMoveAssignOpIfNeeded(bwOptional_DeleteMoveAssignOpIfNeeded&&) = default;
  bwOptional_DeleteMoveAssignOpIfNeeded& operator=(const bwOptional_DeleteMoveAssignOpIfNeeded&) =
      default;
  bwOptional_DeleteMoveAssignOpIfNeeded& operator=(bwOptional_DeleteMoveAssignOpIfNeeded&&) =
      delete;
};

}  // namespace bwOptional_internal
}  // namespace bWidgets

/** \endcond */
