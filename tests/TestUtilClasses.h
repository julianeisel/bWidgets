#pragma once

#include <cassert>

namespace TestUtilClasses {

class SimpleFunction {
 public:
  constexpr int foo() const
  {
    return 23;
  }
};

class SimpleFunctionWithVariants {
 public:
  enum RefType {
    CONST_LREF,
    NON_CONST_LREF,
    CONST_RREF,
    NON_CONST_RREF,
  };
  constexpr RefType foo() const&
  {
    return CONST_LREF;
  }
  RefType foo() &
  {
    return NON_CONST_LREF;
  }
  constexpr RefType foo() const&&
  {
    return CONST_RREF;
  }
  RefType foo() &&
  {
    return NON_CONST_RREF;
  }
};

class ConstexprConstructors {
 public:
  int _value = 0;

  constexpr ConstexprConstructors() = default;
  constexpr ConstexprConstructors(int value) : _value(value)
  {
  }
  constexpr ConstexprConstructors(const ConstexprConstructors&) = default;
  constexpr ConstexprConstructors(ConstexprConstructors&&) = default;

  constexpr bool operator==(const ConstexprConstructors& other)
  {
    return this->_value == other._value;
  }
};

class NoConstructors {
 public:
  NoConstructors() = delete;
  NoConstructors(const NoConstructors&) = delete;
  NoConstructors& operator=(const NoConstructors&) = delete;
};

/**
 * * Counts constructor calls or instances.
 * * Stores integer in _value.
 * * Implicitly constructible from int.
 */
class NonTrivialConstructors {
 public:
  static int tot_default_ctor_calls;
  static int tot_copy_ctor_calls;
  static int tot_move_ctor_calls;
  static int tot_int_ctor_calls;
  static int tot_dtor_calls;
  static int tot_instances;
  int _value = 0;

  NonTrivialConstructors()
  {
    tot_default_ctor_calls++;
    tot_instances++;
  }
  NonTrivialConstructors(int value)
  {
    _value = value;
    tot_int_ctor_calls++;
    tot_instances++;
  }
  NonTrivialConstructors(const NonTrivialConstructors& other)
  {
    _value = other._value;
    tot_copy_ctor_calls++;
    tot_instances++;
  }
  NonTrivialConstructors(NonTrivialConstructors&& other)
  {
    _value = other._value;
    other._value = -1;
    tot_move_ctor_calls++;
    tot_instances++;
  }
  ~NonTrivialConstructors()
  {
    tot_dtor_calls++;
    tot_instances--;
  }

  NonTrivialConstructors& operator=(const NonTrivialConstructors&) = default;

  static void reset_constructors()
  {
    tot_default_ctor_calls = tot_int_ctor_calls = 0;
    tot_copy_ctor_calls = tot_move_ctor_calls = 0;
  }
  static void reset()
  {
    reset_constructors();
    tot_dtor_calls = tot_instances = 0;
  }
  static void assert_no_instance()
  {
    assert(tot_instances == 0);
    assert(tot_default_ctor_calls == 0);
    assert(tot_int_ctor_calls == 0);
    assert(tot_copy_ctor_calls == 0);
    assert(tot_move_ctor_calls == 0);
  }
};

class NoCopyConstructor : public NonTrivialConstructors {
 public:
  using NonTrivialConstructors::NonTrivialConstructors;

  NoCopyConstructor() = default;
  NoCopyConstructor(const NoCopyConstructor&) = delete;
  NoCopyConstructor(NoCopyConstructor&&) = default;
};
class NoMoveConstructor : public NonTrivialConstructors {
 public:
  using NonTrivialConstructors::NonTrivialConstructors;

  NoMoveConstructor() = default;
  NoMoveConstructor(const NoMoveConstructor&) = default;
  NoMoveConstructor(NoMoveConstructor&&) = delete;
};
class NoCopyAssignmentOperator {
 public:
  // Just delete copy-assign op, don't inherit from NonTrivialConstructors or
  // default the constructors. Can be added if needed.
  NoCopyAssignmentOperator& operator=(const NoCopyAssignmentOperator&) = delete;
};
class NoMoveAssignmentOperator {
 public:
  // Just delete move-assign op, don't inherit from NonTrivialConstructors or
  // default the constructors. Can be added if needed.
  NoMoveAssignmentOperator& operator=(NoMoveAssignmentOperator&&) = delete;
};

class ExplicitConstructibleFromInt : public NonTrivialConstructors {
 public:
  explicit ExplicitConstructibleFromInt(int i) : NonTrivialConstructors(i)
  {
  }
};

class ExplicitNonTrivialConstructors : public NonTrivialConstructors {
 public:
  using NonTrivialConstructors::NonTrivialConstructors;

  explicit ExplicitNonTrivialConstructors() = default;
  explicit ExplicitNonTrivialConstructors(const ExplicitNonTrivialConstructors&) = default;
  explicit ExplicitNonTrivialConstructors(ExplicitNonTrivialConstructors&&) = default;
};

class CustomSwap {
 public:
  CustomSwap(int i) : _value(i)
  {
    tot_int_ctor_calls++;
  }
  CustomSwap(CustomSwap&& other) : _value(other._value)
  {
    tot_move_ctor_calls++;
  }
  ~CustomSwap()
  {
    tot_dtor_calls++;
  }
  friend void swap(CustomSwap& a, CustomSwap& b)
  {
    std::swap(a._value, b._value);
    tot_swap_calls++;
  }

  static void reset()
  {
    tot_int_ctor_calls = tot_move_ctor_calls = 0;
    tot_dtor_calls = tot_swap_calls = 0;
  }

  static void assert_no_instance()
  {
    assert(tot_int_ctor_calls == 0);
    assert(tot_move_ctor_calls == 0);
    assert(tot_dtor_calls == 0);
    assert(tot_swap_calls == 0);
  }

  static int tot_int_ctor_calls;
  static int tot_move_ctor_calls;
  static int tot_dtor_calls;
  static int tot_swap_calls;
  int _value;
};

}  // namespace TestUtilClasses
