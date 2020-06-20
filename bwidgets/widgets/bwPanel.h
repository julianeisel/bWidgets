#pragma once

#include "bwContainerWidget.h"

namespace bWidgets {

class bwPanel : public bwContainerWidget {
  friend class bwPanelHandler;

 public:
  bwPanel(const bwScreenGraph::ContainerNode& node,
          std::string label,
          std::optional<unsigned int> header_height_hint = std::nullopt);

  void draw(class bwStyle& style) override;

  void registerProperties() override;

  const std::string* getLabel() const override;
  bool childrenVisible() const override;

  std::unique_ptr<bwScreenGraph::EventHandler> createHandler() override;

  unsigned int getHeaderHeightHint() const;
  unsigned int header_height;

  enum class State {
    OPEN,
    CLOSED,
  } panel_state{State::OPEN};

 private:
  void drawHeader(class bwStyle& style) const;
  bwRectanglePixel getHeaderRectangle() const;
  bool isCoordinateInsideHeader(const bwPoint& point) const;

  std::string label;

 public:
  bool draw_separator = false;
};

}  // namespace bWidgets
