#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwScrollBar : public bwAbstractButton {
  friend class bwScrollView;

 public:
  bwScrollBar(unsigned int width_hint = 0, unsigned int height_hint = 0);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(bwStyle& style) override;
  auto matches(const bwWidget& other) const -> bool override;
  auto alwaysPersistent() const -> bool override;

  void createState() override;
  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  int getScrollOffset();
  void setScrollOffset(int value);

  static auto getInnerRect(bwScrollBar& scrollbar) -> bwRectanglePixel;

 private:
  float ratio_ = 0.0f;  // Ration between content and area height (max 1.0f).
};

}  // namespace bWidgets
