#pragma once

#include <typeinfo>

#include "bwDistance.h"
#include "bwFunctorInterface.h"
#include "bwRectangle.h"
#include "bwStyleProperties.h"
#include "screen_graph/EventHandler.h"

namespace bWidgets {

class bwEvent;
class bwMouseButtonEvent;
class bwMouseButtonDragEvent;

/**
 * \brief Abstract base class that all widgets derive from.
 */
class bwWidget {
 public:
  enum class State {
    NORMAL = 0,
    HIGHLIGHTED,
    SUNKEN,

    STATE_TOT
  } state;

  bwWidget(const std::string& identifier,
           bwOptional<unsigned int> width_hint,
           bwOptional<unsigned int> height_hint);
  virtual ~bwWidget() = default;

  virtual bool isCoordinateInside(const bwPoint& point) const;

  virtual void draw(class bwStyle& style) = 0;

  const std::string& getIdentifier() const;
  virtual const std::string* getLabel() const;

  virtual bool canAlign() const;

  virtual bwPtr<bwScreenGraph::EventHandler> createHandler() = 0;

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

  bwStyleProperties style_properties;

 protected:
  std::string identifier;

  void initialize();
  virtual void registerProperties();
};

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
 * \return The derrived widget class of type `T*` or `nullptr` if cast is not valid (whereby T is
 *         the "raw" type requested, without pointer).
 */
template<class T, class _RawT = typename std::remove_pointer<T>::type>
inline _RawT* widget_cast(bwWidget& widget)
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<_RawT*>(&widget);
}

/**
 * See \ref widget_cast(bwWidget& widget)
 * \return The derrived widget class of type `const T*` or `nullptr` if cast is not valid (whereby
 *         T is the "raw" type requested, without const qualifier or pointer).
 */
template<class T,
         class _RawT = typename std::remove_pointer<typename std::remove_const<T>::type>::type>
inline const _RawT* widget_cast(const bwWidget& widget)
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<const _RawT*>(&widget);
}

/**
 * See \ref widget_cast(bwWidget& widget)
 * \return The derrived widget class of type `T*` or `nullptr` if cast is not valid (whereby T is
 *         the "raw" type requested, without pointer).
 */
template<class T, class _RawT = typename std::remove_pointer<T>::type>
inline _RawT* widget_cast(bwWidget* widget)
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<_RawT*>(widget);
}

/**
 * See \ref widget_cast(bwWidget& widget)
 * \return The derrived widget class of type `const T*` or `nullptr` if cast is not valid (whereby
 *         T is the "raw" type requested, without const qualifier or pointer).
 */
template<class T,
         class _RawT = typename std::remove_pointer<typename std::remove_const<T>::type>::type>
inline const _RawT* widget_cast(const bwWidget* widget)
{
  static_assert(std::is_base_of<bwWidget, _RawT>::value, "Type is not a widget");

  return dynamic_cast<const _RawT*>(widget);
}

}  // namespace bWidgets
