#pragma once

#include "bwTextBox.h"

namespace bWidgets {

class bwNumberSlider : public bwTextBox {
 public:
  bwNumberSlider(const unsigned int width_hint = 0, const unsigned int height_hint = 0);

  void draw(class bwStyle &style) override;

  void mousePressEvent(const MouseButton button, const bwPoint &location) override;
  void mouseReleaseEvent(const MouseButton button, const bwPoint &location) override;
  void mouseClickEvent(const MouseButton button, const bwPoint &location) override;
  void mouseDragEvent(const MouseButton button, const bwDistance drag_distance) override;

  void setValue(float value);
  float getValue() const;
  void setMinMax(float min, float max);

  bwPtr<bwFunctorInterface> apply_functor{nullptr};

 private:
  std::string valueToString(unsigned int precision) const;
  void drawValueIndicator(class bwPainter &painter, class bwStyle &style) const;
  float calcValueIndicatorWidth(class bwStyle &style) const;

  /**
   * Support multiple numeric types. bwNumberSlider could be made
   * a template class for this, but using union is just fine.
   */
  union {
    // float
    struct {
      float value;
      float min, max;
      // Initial value before starting to edit.
      float initial_value;
      unsigned int precision;
    };
    // struct { int value; ...}
    // struct { char value; ...}
    // ...
  };
};

}  // namespace bWidgets
