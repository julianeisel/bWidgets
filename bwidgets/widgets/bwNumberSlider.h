#pragma once

#include "bwTextBox.h"

namespace bWidgets {

class bwPainter;
class bwStyle;

class bwNumberSlider : public bwTextBox {
 public:
  friend class bwNumberSliderHandler;

  bwNumberSlider(std::optional<unsigned int> width_hint = std::nullopt,
                 std::optional<unsigned int> height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(bwStyle& style) override;

  auto createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  auto setValue(float value) -> bwNumberSlider&;
  auto getValue() const -> float;
  auto setMinMax(float min, float max) -> bwNumberSlider&;

  std::unique_ptr<bwFunctorInterface> apply_functor{nullptr};

 private:
  auto valueToString(unsigned int precision) const -> std::string;
  void drawValueIndicator(bwPainter& painter, bwStyle& style) const;
  auto calcValueIndicatorWidth(class bwStyle& style) const -> float;

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
