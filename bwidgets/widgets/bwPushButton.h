#pragma once

#include <string>

#include "bwAbstractButton.h"

namespace bWidgets {

class bwPushButton : public bwAbstractButton {
 public:
  bwPushButton(const std::string& text,
               bwOptional<unsigned int> width_hint = nullopt,
               bwOptional<unsigned int> height_hint = nullopt);

  const class bwIconInterface* getIcon() const override;
  bwPushButton& setIcon(const class bwIconInterface&);

  bool canAlign() const override;

 private:
  const class bwIconInterface* icon{nullptr};
};

}  // namespace bWidgets
