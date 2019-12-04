#pragma once

#include <typeinfo>

#include "bwDistance.h"
#include "bwFunctorInterface.h"
#include "bwRectangle.h"
#include "bwStyleProperties.h"
#include "screen_graph/EventHandler.h"

namespace bWidgets {

class bwMouseButtonEvent;
class bwMouseButtonDragEvent;

/**
 * \brief Abstract base class that all widgets derive from.
 *
 * The widget base class inherits from the #bwScreenGraph::EventHandler API,
 * allowing it to implement event-listeners. This base class implements default
 * listeners, which simply do nothing. To make widgets react to a certain event,
 * override the corresponding listener in the specific widget type.
 *
 * More information on how event handling works in bWidgets can be found in the
 * [design overview](md_docs_bWidgets_design_overview.html).
 */
class bwWidget : public bwScreenGraph::EventHandler {
 public:
  // TODO we might not need this.
  enum WidgetType {
    WIDGET_TYPE_CHECKBOX,
    WIDGET_TYPE_LABEL,
    WIDGET_TYPE_NUMBER_SLIDER,
    WIDGET_TYPE_PUSH_BUTTON,
    WIDGET_TYPE_RADIO_BUTTON,
    WIDGET_TYPE_SCROLL_BAR,
    WIDGET_TYPE_TEXT_BOX,

    WIDGET_TYPE_PANEL,

    WIDGET_TYPE_TOT
  } type;

  enum WidgetState {
    STATE_NORMAL = 0,
    STATE_HIGHLIGHTED,
    STATE_SUNKEN,

    STATE_TOT
  } state;

  bwWidget(const WidgetType type,
           std::string identifier,
           const unsigned int width_hint = 0,
           const unsigned int height_hint = 0);
  virtual ~bwWidget() override = default;

  virtual bool isCoordinateInside(const bwPoint& point) const;

  virtual void draw(class bwStyle& style) = 0;

  // Events
  void onMouseEnter() override;
  void onMouseLeave() override;
  void onMousePress(bwMouseButtonEvent& event) override;
  void onMouseRelease(bwMouseButtonEvent& event) override;
  void onMouseClick(bwMouseButtonEvent& event) override;
  void onMouseDrag(bwMouseButtonDragEvent& event) override;

  const std::string& getIdentifier() const;
  virtual const std::string* getLabel() const;

  virtual bool canAlign() const;

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
