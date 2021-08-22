#pragma once

#include "bwContainerWidget.h"

namespace bWidgets {

class bwStyle;

class bwPanel : public bwContainerWidget {
  friend class bwPanelHandler;

 public:
  enum class State {
    OPEN,
    CLOSED,
  };

  bwPanel(const bwScreenGraph::ContainerNode& node,
          std::string label,
          std::optional<unsigned int> header_height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(class bwStyle& style) override;

  auto matches(const bwWidget& other) const -> bool override;
  auto alwaysPersistent() const -> bool override;
  void copyState(const bwWidget& from) override;

  void registerProperties() override;

  auto getLabel() const -> const std::string* override;
  auto childrenVisible() const -> bool override;

  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  unsigned int getHeaderHeightHint() const;

  unsigned int header_height;
  State panel_state{State::OPEN};

 private:
  void drawHeader(class bwStyle& style) const;
  auto getHeaderRectangle() const -> bwRectanglePixel;
  auto isCoordinateInsideHeader(const bwPoint& point) const -> bool;

  std::string label;

 public:
  bool draw_separator = false;
};

}  // namespace bWidgets
