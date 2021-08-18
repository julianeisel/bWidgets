#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"

namespace bWidgets {

class bwIconInterface;
class bwStyle;

class bwLabel : public bwWidget {
 public:
  bwLabel(std::string text = "",
          std::optional<unsigned int> width_hint = std::nullopt,
          std::optional<unsigned int> height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(bwStyle& style) override;
  auto matches(const bwWidget& widget) const -> bool override;

  void registerProperties() override;
  auto createHandler(bwScreenGraph::Node& node)
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  auto getLabel() const -> const std::string* override;
  auto setLabel(const std::string&) -> bwLabel&;
  auto setIcon(const bwIconInterface& icon_interface) -> bwLabel&;

 private:
  std::string text;
  const class bwIconInterface* icon{nullptr};

  bwColor text_color;
};

}  // namespace bWidgets
