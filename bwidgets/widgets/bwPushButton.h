#pragma once

#include <string>

#include "bwAbstractButton.h"

namespace bWidgets {

class bwPushButton : public bwAbstractButton {
 public:
  bwPushButton(const std::string& text,
               std::optional<unsigned int> width_hint = std::nullopt,
               std::optional<unsigned int> height_hint = std::nullopt);

  auto matches(const bwWidget& other) const -> bool override;

  auto getIcon() const -> const bwIconInterface* override;
  auto setIcon(const class bwIconInterface&) -> bwPushButton&;

  auto canAlign() const -> bool override;

 private:
  const class bwIconInterface* icon{nullptr};
};

}  // namespace bWidgets
