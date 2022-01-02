#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets {

class bwTextBoxState;

class bwTextBox : public bwWidget {
  friend class bwTextBoxHandler;

 public:
  bwTextBox(std::optional<unsigned int> width_hint = std::nullopt,
            std::optional<unsigned int> height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(class bwStyle& style) override;
  auto matches(const bwWidget& other) const -> std::optional<bool> override;

  void registerProperties() override;
  auto setText(const std::string& value) -> bwTextBox&;
  auto getLabel() const -> const std::string* override;

  /* State */
  auto isTextEditing() const -> bool;
  auto getSelectionRectangle() const -> bwRectanglePixel&;

  auto canAlign() const -> bool override;

  auto createState() const -> std::unique_ptr<bwWidgetState> override;
  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  bwRectanglePixel selection_rectangle;

 protected:
  std::string text;

  auto state() const -> bwTextBoxState&;

 public:
  bwWidgetBaseStyle
      base_style;  // XXX public for setWidgetStyle. Should only be temporarily needed.
};

struct bwTextBoxState : public bwWidgetState {
  bwRectanglePixel selection_rectangle;
  bool is_text_editing = false;
};

class bwTextBoxHandler : public bwScreenGraph::WidgetEventHandler<bwTextBox> {
 public:
  bwTextBoxHandler(bwScreenGraph::Node& node);
  virtual ~bwTextBoxHandler() = default;

  void onMouseEnter(bwEvent&) override;
  void onMouseLeave(bwEvent&) override;
  void onMousePress(bwMouseButtonEvent&) override;

  void startTextEditing();
  void endTextEditing();

  bool is_dragging = false;
};

}  // namespace bWidgets
