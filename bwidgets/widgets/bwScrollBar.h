#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwScrollBar : public bwAbstractButton {
 public:
  bwScrollBar(unsigned int width_hint = 0, unsigned int height_hint = 0);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(bwStyle& style) override;
  auto matches(const bwWidget& other) const -> bool override;
  void copyState(const bwWidget& from) override;

  auto createHandler(bwScreenGraph::Node& node)
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  float ratio = 0.0f;  // Ration between content and area height (max 1.0f).
  int scroll_offset = 0;

 private:
  void setScrollOffset(int value);
};

}  // namespace bWidgets
