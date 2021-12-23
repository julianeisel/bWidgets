#pragma once

#include <optional>
#include <typeinfo>

#include "bwDistance.h"
#include "bwFunctorInterface.h"
#include "bwRectangle.h"
#include "bwStyleProperties.h"
#include "bwWidgetState.h"
#include "screen_graph/EventHandler.h"

namespace bWidgets {

class bwEvent;
class bwMouseButtonEvent;
class bwMouseButtonDragEvent;
class bwStyle;

namespace bwScreenGraph {
class EventHandler;
class Node;
class WidgetNode;
}  // namespace bwScreenGraph

/**
 * \brief Abstract base class that all widgets derive from.
 */
class bwWidget {
  friend class bwScreenGraph::WidgetNode;

 public:
  bwWidget(std::optional<unsigned int> width_hint, std::optional<unsigned int> height_hint);
  virtual ~bwWidget() = default;

  /* Disable these constructors/operators. Not really needed and would add complexity for
   * sub-classes. */
  bwWidget() = delete;
  bwWidget(const bwWidget&) = delete;
  bwWidget(bwWidget&&) = delete;
  auto operator=(const bwWidget&) = delete;
  auto operator=(bwWidget&&) = delete;

  template<class _WidgetStateType> /* Defined below. */
  auto getState() const -> _WidgetStateType&;
  auto getBaseState() const -> bwWidgetState::BaseState;
  auto setBaseState(bwWidgetState::BaseState) -> bwWidget&;
  auto hide(bool _hidden = true) -> bwWidget&;
  auto isHidden() -> bool;

  virtual auto getTypeIdentifier() const -> std::string_view = 0;

  virtual void draw(bwStyle& style) = 0;
  virtual auto isCoordinateInside(const bwPoint& point) const -> bool;
  virtual auto getLabel() const -> const std::string*;
  virtual auto canAlign() const -> bool;

  virtual auto createState() const -> std::unique_ptr<bwWidgetState>;
  virtual auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> = 0;

  /**
   * Final rectangle defining the widget bounding-box.
   * \note This really is assumed to be the final bounding-box, bwStyle.dpi_fac will not get
   * applied to it.
   */
  bwRectanglePixel rectangle;

  /**
   * Define size hints for the widget. bWidgets itself doesn't do anything with it. The actual
   * application can use it for its layout calculations or simply ignore it. For bWidgets all
   * that matters is the final \a rectangle. Like the name suggests it's really just a hint.
   */
  unsigned int width_hint, height_hint;

  bwStyleProperties style_properties;

 protected:
  /**
   * Compare this widget to \a other, to see if they represent the same data. Crucial for
   * identifying widgets, which again is crucial for keeping state over redraws (where the
   * screen graph is reconstructed).
   *
   * Can assume the widget type was already checked by the caller (by calling
   * #WidgetNode::matchesType()).
   */
  virtual auto matches(const bwWidget& other) const -> bool = 0;

  void initialize();
  virtual void registerProperties();

 private:
  /**
   * Hint if widget was explicitly hidden. bWidgets itself doesn't do
   * anything with it (yet). The actual application can use it for its layout
   * calculations or simply ignore it.
   * \note This should not be used to mark a widget as not visible (e.g.
   *       because it's scrolled out of view). Only use it to hide a widget
   *       after an explicit user action that makes the button irrelevant or
   *       inappropriate (e.g. a usage hint that should only be shown if a
   *       specific option is enabled).
   */
  bool hidden{false};
  /**
   * \warning Never access directly, this is lazy-created! Use #bwWidget::getState() instead.
   */
  std::unique_ptr<bwWidgetState> state_;
};

/**
 * Allocate and initialize a new widget of given type. All arguments passed to this will be
 * forwarded to the widget's constructor.
 *
 * \note Not really needed currently, it just wraps a `std::make_unique()` call. Leaving it in case
 *       we need to do further initialization in future.
 */
template<class _WidgetType, class... _Args>
std::unique_ptr<_WidgetType> WidgetNew(_Args&&... __args)
{
  return std::make_unique<_WidgetType>(std::forward<_Args>(__args)...);
}

template<class _WidgetStateType> auto bwWidget::getState() const -> _WidgetStateType&
{
  /* Lazy-create state, so we don't have to enforce this on construction. */
  if (!state_) {
    /* Casting away const is fine here, since it doesn't modify the object in an observable way.
     * Just lazy-creates the state. */
    const_cast<bwWidget*>(this)->state_ = this->createState();
  }

  if constexpr (std::is_same<_WidgetStateType, bwWidgetState>::value) {
    return *state_;
  }
  else {
    static_assert(std::is_base_of_v<bwWidgetState, _WidgetStateType>,
                  "Type should derive from bwWidgetState");

    assert(dynamic_cast<_WidgetStateType*>(state_.get()) != nullptr);
    return dynamic_cast<_WidgetStateType&>(*state_);
  }
}

/**
 * Try to dynamically cast a widget from one widget type to another. Use-case is not just to
 * perform the cast itself, but to also to check if a widget is of a specific type.
 *
 * There are a couple of reasons why types are checked this way:
 * * Using \ref bwWidget::identifier for this would imply string comparisons - which scale badly.
 * * The most practical alternative would probably be a type enum. To add a widget, the central
 *   enum definition would have to be extended. This makes it impossible to define a widget in a
 *   single file.
 * * Enums can't be modified from external code to define custom widget types (types not definied
 *   within bWidgets). So for these a "custom" enum item would be needed. If this is set, an
 *   alternative type check mechanism would be needed, likely the slower string comparison.
 * * Using enums leads to more verbose and less safe checks. If an enum value says the widget is of
 *   a certain type, the code will assume it was also constructed for this type. It typically
 *   performs a static cast, which may work fine in most cases, but is technically unsafe still.
 *   Using `dynamic_cast<>()`, the compiler ensures a type-safe operation.
 *
 * Retains const-correctness and always returns a pointer. E.g. `widget_cast<bwFoo>(const_object)`
 * returns a `const bwFoo*` type.
 *
 * \note By design, bWidgets should not need much type casting or type checking like this. Widget
 *       types, composed together with other custom objects (like widget specific event handlers)
 *       through the screen-graph node, should be able to define comprehensive custom behavior.
 *       Sometimes it's still necessary to customize behavior through type checks from outside the
 *       widget type definition though. At least for now.
 *
 * \return The derived widget class of type `T*` or `nullptr` if cast is not valid (whereby T is
 *         the "raw" type requested, without pointer).
 */
template<class T, class _RawT = typename std::remove_pointer<T>::type>
inline auto widget_cast(bwWidget& widget) -> _RawT*
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<_RawT*>(&widget);
}

/**
 * See \ref widget_cast(bwWidget& widget)
 * \return The derived widget class of type `const T*` or `nullptr` if cast is not valid (whereby
 *         T is the "raw" type requested, without const qualifier or pointer).
 */
template<class T,
         class _RawT = typename std::remove_pointer<typename std::remove_const<T>::type>::type>
inline auto widget_cast(const bwWidget& widget) -> const _RawT*
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<const _RawT*>(&widget);
}

/**
 * See \ref widget_cast(bwWidget& widget)
 * \return The derived widget class of type `T*` or `nullptr` if cast is not valid (whereby T is
 *         the "raw" type requested, without pointer).
 */
template<class T, class _RawT = typename std::remove_pointer<T>::type>
inline auto widget_cast(bwWidget* widget) -> _RawT*
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<_RawT*>(widget);
}

/**
 * See \ref widget_cast(bwWidget& widget)
 * \return The derived widget class of type `const T*` or `nullptr` if cast is not valid (whereby
 *         T is the "raw" type requested, without const qualifier or pointer).
 */
template<class T,
         class _RawT = typename std::remove_pointer<typename std::remove_const<T>::type>::type>
inline auto widget_cast(const bwWidget* widget) -> const _RawT*
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<const _RawT*>(widget);
}

}  // namespace bWidgets
