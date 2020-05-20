#pragma once

#include "bwTextBox.h"

namespace bWidgets {

class bwNumberSlider : public bwTextBox {
 public:
  friend class bwNumberSliderHandler;

  bwNumberSlider(bwOptional<unsigned int> width_hint = nullopt,
                 bwOptional<unsigned int> height_hint = nullopt);

  void draw(class bwStyle& style) override;

  bwPtr<bwScreenGraph::EventHandler> createHandler() override;

  void setValue(float value);
  float getValue() const;
  void setMinMax(float min, float max);

  bwPtr<bwFunctorInterface> apply_functor{nullptr};

 private:
  std::string valueToString(unsigned int precision) const;
  void drawValueIndicator(class bwPainter& painter, class bwStyle& style) const;
  float calcValueIndicatorWidth(class bwStyle& style) const;

  /**
   * Support multiple numeric types. bwNumberSlider could be made
   * a template class for this, but using union is just fine.
   */
  union {
    // float
    struct {
      float value;
      float min, max;
      unsigned int precision;
    };
    // struct { int value; ...}
    // struct { char value; ...}
    // ...
  };
};

}  // namespace bWidgets
