#pragma once

#include <memory>
#include <string>

#include "bwUtil.h"
#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

class bwIconInterface;

/**
 * \brief Base class for button like widgets.
 */
class bwAbstractButton : public bwWidget {
 public:
  void draw(class bwStyle& style) override;
  auto matches(const bwWidget& other) const -> bool override;

  void registerProperties() override;

  auto getLabel() const -> const std::string* override;
  auto setLabel(const std::string& label) -> bwAbstractButton&;
  virtual auto getIcon() const -> const bwIconInterface*;

  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  /**
   * Function object called when applying changes to widget.
   */
  std::unique_ptr<bwFunctorInterface> apply_functor{nullptr};

  unsigned int rounded_corners;

 protected:
  // Protected constructor to force calling through inherited class (pseudo abstract).
  bwAbstractButton(const std::string& text,
                   std::optional<unsigned int> width_hint = std::nullopt,
                   std::optional<unsigned int> height_hint = std::nullopt);
  void apply();

  std::string text;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

// ------------------ Handling ------------------

/**
 * PImpl (Pointer to implementation) class to keep the handler implementation in the source file.
 * Since the actual class is a template this would have to be in the header otherwise.
 */
class bwAbstractButtonHandlerPImpl {
 public:
  void onMouseEnter(bwEvent& event, bwAbstractButton& button);
  void onMouseLeave(bwEvent& event, bwAbstractButton& button);
  void onMousePress(bwMouseButtonEvent& event, bwAbstractButton& button);
  void onMouseRelease(bwMouseButtonEvent& event, bwAbstractButton& button);

  void apply(bwAbstractButton& button);
};

template<class _WidgetType = bwAbstractButton>
class bwAbstractButtonHandler : public bwScreenGraph::WidgetEventHandler<_WidgetType> {
  std::unique_ptr<bwAbstractButtonHandlerPImpl> pimpl;

 public:
  bwAbstractButtonHandler(bwScreenGraph::Node& node)
      : bwScreenGraph::WidgetEventHandler<_WidgetType>(node)
  {
  }

  virtual ~bwAbstractButtonHandler() = default;

  void onMouseEnter(bwEvent& event) override
  {
    return pimpl->onMouseEnter(event, Widget());
  }
  void onMouseLeave(bwEvent& event) override
  {
    return pimpl->onMouseLeave(event, Widget());
  }
  void onMousePress(bwMouseButtonEvent& event) override
  {
    return pimpl->onMousePress(event, Widget());
  }
  void onMouseRelease(bwMouseButtonEvent& event) override
  {
    return pimpl->onMouseRelease(event, Widget());
  }

  void apply()
  {
    return pimpl->apply(Widget());
  }

  using bwScreenGraph::WidgetEventHandler<_WidgetType>::Widget;
};

}  // namespace bWidgets
