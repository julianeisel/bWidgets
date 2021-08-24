#pragma once

#include <limits>

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
  auto matches(const bwWidget& other) const -> bool override;
  void copyState(const bwWidget& from) override;

  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

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
  union ValueInfo {
    ValueInfo()
    {
    }

    // float
    struct {
      float value = 0.0f;
      float min = std::numeric_limits<float>::min();
      float max = std::numeric_limits<float>::max();
      uint precision = 2;
    };
    // struct { int value; ...}
    // struct { char value; ...}
    // ...
  } value_info;
};

}  // namespace bWidgets
