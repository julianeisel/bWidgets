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

  std::unique_ptr<bwScreenGraph::EventHandler> createHandler() override;

  /**
   * Function object called when applying changes to widget.
   */
  std::unique_ptr<bwFunctorInterface> apply_functor{nullptr};

  unsigned int rounded_corners;

 protected:
  // Protected constructor to force calling through inherited class (pseudo abstract).
  bwAbstractButton(const std::string& text,
                   const std::string& identifier,
                   std::optional<unsigned int> width_hint = std::nullopt,
                   std::optional<unsigned int> height_hint = std::nullopt);
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
