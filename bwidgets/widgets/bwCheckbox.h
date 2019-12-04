#pragma once

#include "bwAbstractButton.h"

namespace bWidgets {

class bwCheckbox : public bwAbstractButton {
 public:
  bwCheckbox(const std::string& text, unsigned int width_hint, unsigned int height_hint);

  void draw(class bwStyle& style) override;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;

  bool isChecked() const;

 private:
  bool isInsideCheckbox(const bwPoint& point) const;

  bwRectanglePixel getCheckboxRectangle() const;
  bwRectanglePixel getTextRectangle(const bwRectanglePixel& checkbox_rectangle) const;
};

}  // namespace bWidgets
