#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwScrollBar : public bwAbstractButton {
 public:
  bwScrollBar(unsigned int width_hint = 0, unsigned int height_hint = 0);

  void draw(bwStyle& style) override;

  std::unique_ptr<bwScreenGraph::EventHandler> createHandler() override;

  float ratio = 0.0f;  // Ration between content and area height (max 1.0f).
  int scroll_offset = 0;

 private:
  void setScrollOffset(int value);
};

}  // namespace bWidgets
