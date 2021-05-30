#pragma once

#include <memory>

namespace bWidgets {

/**
 * \brief Abstract function object class.
 *
 * We use this to avoid having to use function pointers with ugly void
 * pointers to pass type-generic data. Instead, just derive from this
 * interface and add needed data to the derived class.
 *
 * Example implementation:
 * \code{.cc}
 * class WidgetApplyFunctor : public bwFunctorInterface
 * {
 *   bwWidget& some_widget;
 *
 * public:
 *   WidgetApplyFunctor(bwWidget& widget) :
 *       some_widget(widget)
 *   {
 *   }
 *
 *   inline void operator()() override
 *   {
 *     some_widget.do_stuff();
 *   }
 * };
 * \endcode
 */
class bwFunctorInterface {
 public:
  virtual ~bwFunctorInterface() = default;

  virtual void operator()() = 0;
  virtual auto matches(const bwFunctorInterface& other) const -> bool = 0;
};

/**
 * Utility to compare to #bwFunctorInterface pointers for equality, including value equality.
 * Should always be done the same way.
 * Considers the case where both functors are `nullptr` as not equal.
 */
inline auto compareFunctors(const bwFunctorInterface* a, const bwFunctorInterface* b) -> bool
{
  if (a && (a == b)) {
    return true;
  }
  /* If they are not equal, and one is nullptr, there's no match. */
  if (!a || !b) {
    return false;
  }
  /* Lastly, do value comparison. */
  return a->matches(*b);
}

/**
 * Convenience wrapper using `std::unique_ptr` references.
 */
inline auto compareFunctors(const std::unique_ptr<bwFunctorInterface>& a,
                            const std::unique_ptr<bwFunctorInterface>& b) -> bool
{
  return compareFunctors(a.get(), b.get());
}

}  // namespace bWidgets
