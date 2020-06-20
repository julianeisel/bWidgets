#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwCheckbox : public bwAbstractButton {
 public:
  bwCheckbox(const std::string& text,
             std::optional<unsigned int> width_hint = std::nullopt,
             std::optional<unsigned int> height_hint = std::nullopt);

  void draw(class bwStyle& style) override;

  std::unique_ptr<bwScreenGraph::EventHandler> createHandler() override;

  bool isChecked() const;

 private:
  bool isInsideCheckbox(const bwPoint& point) const;

  bwRectanglePixel getCheckboxRectangle() const;
  bwRectanglePixel getTextRectangle(const bwRectanglePixel& checkbox_rectangle) const;
};

}  // namespace bWidgets
