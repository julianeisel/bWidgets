#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwScrollBarState;

class bwScrollBar : public bwAbstractButton {
  friend class bwScrollView;

 public:
  bwScrollBar(unsigned int width_hint = 0, unsigned int height_hint = 0);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(bwStyle& style) override;
  auto matches(const bwWidget& other) const -> std::optional<bool> override;

  auto createState() const -> std::unique_ptr<bwWidgetState> override;
  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  int getScrollOffset();
  void setScrollOffset(int value);

  static auto getInnerRect(bwScrollBar& scrollbar) -> bwRectanglePixel;

 private:
  float ratio_ = 0.0f;  // Ration between content and area height (max 1.0f).

  auto state() const -> bwScrollBarState&;
};

}  // namespace bWidgets
