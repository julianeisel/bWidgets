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
  // TODO we might not need this.
  enum class Type {
    CHECKBOX,
    LABEL,
    NUMBER_SLIDER,
    PUSH_BUTTON,
    RADIO_BUTTON,
    SCROLL_BAR,
    SCROLL_VIEW,
    BOX,

    PANEL,

    TOT
  } type;

  enum class State {
    NORMAL = 0,
    HIGHLIGHTED,
    SUNKEN,

    STATE_TOT
  } state;

  bwWidget(const Type type,
           const std::string& identifier,
           const unsigned int width_hint = 0,
           const unsigned int height_hint = 0);
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
 * Performs a conditional widget cast from bwWidget to derived class if valid.
 * \return the derrived widget class of type T or nullptr if cast is not valid.
 */
template<class T> inline T widget_cast(bwWidget* widget)
{
  try {
    return dynamic_cast<T>(widget);
  }
  catch (const std::bad_cast&) {
    return nullptr;
  }
}
template<class T> inline T widget_cast(const bwWidget* widget)
{
  try {
    return dynamic_cast<T>(widget);
  }
  catch (const std::bad_cast&) {
    return nullptr;
  }
}

}  // namespace bWidgets
