#pragma once

#include <string>

#include "bwAbstractButton.h"

namespace bWidgets {

class bwPushButton : public bwAbstractButton {
 public:
  bwPushButton(const std::string& text,
               std::optional<unsigned int> width_hint = std::nullopt,
               std::optional<unsigned int> height_hint = std::nullopt);

  const class bwIconInterface* getIcon() const override;
  bwPushButton& setIcon(const class bwIconInterface&);

  bool canAlign() const override;

 private:
  const class bwIconInterface* icon{nullptr};
};

}  // namespace bWidgets
