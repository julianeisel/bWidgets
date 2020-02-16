#pragma once

#include <memory>
#include <string>

#include "bwUtil.h"
#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

/**
 * \brief Base class for button like widgets.
 */
class bwAbstractButton : public bwWidget {
 public:
  void draw(class bwStyle& style) override;
  void registerProperties() override;

  const std::string* getLabel() const override;
  virtual const class bwIconInterface* getIcon() const;

  bwPtr<bwScreenGraph::EventHandler> createHandler() override;

  /**
   * Function object called when applying changes to widget.
   */
  bwPtr<bwFunctorInterface> apply_functor{nullptr};

  unsigned int rounded_corners;

 protected:
  // Protected constructor to force calling through inherited class (pseudo abstract).
  bwAbstractButton(const std::string& text,
                   const Type type,
                   const std::string& identifier,
                   const unsigned int width_hint = 0,
                   const unsigned int height_hint = 0);
  void apply();

  const std::string text;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

class bwAbstractButtonHandler : public bwScreenGraph::EventHandler {
 public:
  bwAbstractButtonHandler(bwAbstractButton& button);
  virtual ~bwAbstractButtonHandler() = default;

  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;

 protected:
  bwAbstractButton& button;

  void apply();
};

}  // namespace bWidgets
