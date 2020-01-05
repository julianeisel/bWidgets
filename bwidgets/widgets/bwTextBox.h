#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

class bwTextBox : public bwWidget {
 public:
  bwTextBox(unsigned int width_hint = 0, unsigned int height_hint = 0);

  void draw(class bwStyle& style) override;
  void registerProperties() override;

  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent&) override;

  void startTextEditing();
  void endTextEditing();

  void setText(const std::string& value);
  const std::string* getLabel() const override;

  bool canAlign() const override;

  bwRectanglePixel selection_rectangle;

 protected:
  std::string text;

  bool is_text_editing = false;
  bool is_dragging = false;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

}  // namespace bWidgets
