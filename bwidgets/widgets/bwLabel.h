#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"

namespace bWidgets {

class bwIconInterface;

class bwLabel : public bwWidget {
 public:
  bwLabel(std::string text = "",
          std::optional<unsigned int> width_hint = std::nullopt,
          std::optional<unsigned int> height_hint = std::nullopt);

  void draw(class bwStyle& style) override;
  void registerProperties() override;
  auto createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  auto getLabel() const -> const std::string* override;
  auto setLabel(const std::string&) -> bwLabel&;
  auto setIcon(const bwIconInterface& icon_interface) -> bwLabel&;

 private:
  std::string text;
  const class bwIconInterface* icon{nullptr};

  bwColor text_color;
};

}  // namespace bWidgets
