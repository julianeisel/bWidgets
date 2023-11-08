#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <variant>

namespace bWidgets {

template<typename DataType> class ReferenceBinding final {
  DataType& value_ref_;

 public:
  explicit ReferenceBinding(DataType&);

  void set(const DataType&);
  auto get() const -> const DataType&;
  /**
   * Checks if the bindings refer to the exact same data by comparing the addresses.
   * \return True if both bindings point to the same data in memory (addresses match).
   */
  template<typename OtherDataType>
  friend auto operator==(const ReferenceBinding<OtherDataType>&,
                         const ReferenceBinding<OtherDataType>&) -> bool;
};

template<typename T> class FunctionalBinding {
 public:
  /* Accessed by #FunctionalBindingNew. **/
  using DataType = T;

  virtual ~FunctionalBinding() = default;

  virtual void set(const DataType&) = 0;
  virtual auto get() const -> DataType = 0;
  virtual auto operator==(const FunctionalBinding&) const -> bool = 0;
};

/**
 * Container that can hold either a reference or a function binding of a given data-type (bool,
 * float, etc.).
 *
 * Note that this is copyable (it's cheap to copy), but copies will still refer to the same
 * underlying data (the same reference binding or custom function binding object).
 */
template<typename T> class Binding {
  /* Note that the function binding is a shared pointer - multiple copies of this binding object
   * may refer to the same functional binding object. */
  std::variant<ReferenceBinding<T>, std::shared_ptr<FunctionalBinding<T>>> binding_;

 public:
  /* Expose type publicly. */
  using DataType = T;

  Binding() = delete;
  /* Explicit so it's clear on the caller side that this is a binding, not just passing a value. */
  explicit Binding(DataType&);
  Binding(std::unique_ptr<FunctionalBinding<T>>);

  Binding(const Binding&) = default;
  Binding(Binding&&) = default;
  ~Binding() = default;

  void set(const DataType&);
  auto get() const -> DataType;
  template<typename Type>
  friend auto operator==(const Binding<Type>&, const Binding<Type>&) -> bool;

 private:
  auto get_fn_binding() const -> FunctionalBinding<T>*;
};

/* -------------------------------------------------------------------- */

template<typename DataType>
ReferenceBinding<DataType>::ReferenceBinding(DataType& ref) : value_ref_(ref)
{
}

template<typename DataType> void ReferenceBinding<DataType>::set(const DataType& value)
{
  value_ref_ = value;
}

template<typename DataType> auto ReferenceBinding<DataType>::get() const -> const DataType&
{
  return value_ref_;
}

template<typename DataType>
auto operator==(const ReferenceBinding<DataType>& a, const ReferenceBinding<DataType>& b) -> bool
{
  return &a.value_ref_ == &b.value_ref_;
}

/* -------------------------------------------------------------------- */

template<typename T>
Binding<T>::Binding(DataType& ref_binding)
    : binding_(std::in_place_type<ReferenceBinding<DataType>>, ref_binding)
{
}

template<typename T>
Binding<T>::Binding(std::unique_ptr<FunctionalBinding<T>> fn_binding)
    : binding_(std::move(fn_binding))
{
}

template<typename T> void Binding<T>::set(const DataType& value)
{
  if (auto* ref_binding = std::get_if<ReferenceBinding<T>>(&binding_)) {
    ref_binding->set(value);
  }
  else if (auto* fn_binding = get_fn_binding()) {
    fn_binding->set(value);
  }
  else {
    assert(0);
  }
}

template<typename T> auto Binding<T>::get() const -> DataType
{
  if (auto* ref_binding = std::get_if<ReferenceBinding<T>>(&binding_)) {
    return ref_binding->get();
  }
  else if (auto* fn_binding = get_fn_binding()) {
    return fn_binding->get();
  }

  assert(0);
  return {};
}

template<typename T> auto Binding<T>::get_fn_binding() const -> FunctionalBinding<T>*
{
  if (auto* fn_binding = std::get_if<std::shared_ptr<FunctionalBinding<T>>>(&binding_)) {
    return fn_binding->get();
  }
  return nullptr;
}

template<typename Type> auto operator==(const Binding<Type>& a, const Binding<Type>& b) -> bool
{
  if (a.binding_.index() != b.binding_.index()) {
    return false;
  }

  /* Function binding is stored as pointer, so needs to dereference for comparison. */
  if (auto* a_fn_binding = a.get_fn_binding()) {
    auto* b_fn_binding = b.get_fn_binding();
    return *a_fn_binding == *b_fn_binding;
  }
  return a.binding_ == b.binding_;
}

/* -------------------------------------------------------------------- */

template<typename BindingType, typename... _Args>
auto FunctionalBindingNew(_Args&&... __args)
    -> std::unique_ptr<FunctionalBinding<typename BindingType::DataType>>
{
  static_assert(std::is_base_of_v<FunctionalBinding<typename BindingType::DataType>, BindingType>,
                "Should inherit from FunctionalBinding");
  return std::make_unique<BindingType>(std::forward<_Args>(__args)...);
}

/* -------------------------------------------------------------------- */

template<class T, class Dummy = void> struct has_binding : std::false_type {
};

template<class T>
struct has_binding<T, std::void_t<typename T::DataBindingType>> : std::true_type {
};

}  // namespace bWidgets
