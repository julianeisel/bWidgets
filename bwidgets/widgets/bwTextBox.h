#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

class bwTextBox : public bwWidget {
  friend class bwTextBoxHandler;

 public:
  bwTextBox(bwOptional<unsigned int> width_hint = nullopt,
            bwOptional<unsigned int> height_hint = nullopt);

  void draw(class bwStyle& style) override;
  void registerProperties() override;

  void setText(const std::string& value);
  const std::string* getLabel() const override;

  bool canAlign() const override;

  std::unique_ptr<bwScreenGraph::EventHandler> createHandler() override;

  bwRectanglePixel selection_rectangle;

 protected:
  std::string text;

  bool is_text_editing = false;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

class bwTextBoxHandler : public bwScreenGraph::EventHandler {
 public:
  bwTextBoxHandler(bwTextBox& textbox);
  virtual ~bwTextBoxHandler() = default;

  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent&) override;

 protected:
  bwTextBox& textbox;
  bool is_dragging = false;

  void startTextEditing();
  void endTextEditing();
};

}  // namespace bWidgets
