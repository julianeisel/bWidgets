#pragma once

#include <memory>

namespace bWidgets {

class TypeErasedBinding {
 public:
  /* Not explicit for convenience. */
  template<typename BindingType> TypeErasedBinding(BindingType&& binding);
  friend inline auto operator==(const TypeErasedBinding& a, const TypeErasedBinding& b) -> bool;

 private:
  struct TypeErasedBase {
    virtual ~TypeErasedBase() = default;
    virtual auto operator==(const TypeErasedBase&) const -> bool = 0;
  };

  template<typename BindingType> struct BindingWrapper : public TypeErasedBase {
    explicit BindingWrapper(const BindingType& binding) : binding_(binding)
    {
    }

    auto operator==(const TypeErasedBase& other_base) const -> bool override
    {
      /* Check the bindings are of the same type (by attempting a dynamic cast) and forward the
       * comparison to the bindings if so. */
      if (const BindingWrapper<BindingType>* other_binding =
              dynamic_cast<const BindingWrapper<BindingType>*>(&other_base)) {
        return binding_ == other_binding->binding_;
      }
      return false;
    }

    /* BindingType is something like `Binding<T>` or `const Binding<T>&` (so this may be
     * either owning or a reference). */
    BindingType binding_;
  };

  std::unique_ptr<struct TypeErasedBase> wrapped_binding_;
};

/* -------------------------------------------------------------------- */

template<typename BindingType>
TypeErasedBinding::TypeErasedBinding(BindingType&& binding)
    : wrapped_binding_(std::make_unique<BindingWrapper<BindingType>>(binding))
{
}

auto operator==(const TypeErasedBinding& a, const TypeErasedBinding& b) -> bool
{
  /* Forward to the type-full wrapped binding. */
  return *a.wrapped_binding_ == *b.wrapped_binding_;
}

}  // namespace bWidgets
