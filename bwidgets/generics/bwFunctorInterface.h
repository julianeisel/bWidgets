#pragma once

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
  virtual auto operator==(const bwFunctorInterface& other) const -> bool = 0;
};

}  // namespace bWidgets
