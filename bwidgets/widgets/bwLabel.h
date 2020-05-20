#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"

namespace bWidgets {

class bwLabel : public bwWidget {
 public:
  bwLabel(std::string text,
          bwOptional<unsigned int> width_hint = nullopt,
          bwOptional<unsigned int> height_hint = nullopt);

  void draw(class bwStyle& style) override;
  void registerProperties() override;
  const std::string* getLabel() const override;
  bwPtr<bwScreenGraph::EventHandler> createHandler() override;

  bwLabel& setIcon(const class bwIconInterface& icon_interface);

 private:
  const std::string text;
  const class bwIconInterface* icon{nullptr};

  bwColor text_color;
};

}  // namespace bWidgets
