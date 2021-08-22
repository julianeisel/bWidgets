#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

class bwTextBox : public bwWidget {
  friend class bwTextBoxHandler;

 public:
  bwTextBox(std::optional<unsigned int> width_hint = std::nullopt,
            std::optional<unsigned int> height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(class bwStyle& style) override;
  auto matches(const bwWidget& other) const -> bool override;

  void registerProperties() override;
  auto setText(const std::string& value) -> bwTextBox&;
  auto getLabel() const -> const std::string* override;

  bool canAlign() const override;

  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

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
  bwTextBoxHandler(bwScreenGraph::Node& node);
  virtual ~bwTextBoxHandler() = default;

  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent&) override;

 protected:
  void startTextEditing();
  void endTextEditing();

  bwTextBox& textbox;
  bool is_dragging = false;
};

}  // namespace bWidgets
