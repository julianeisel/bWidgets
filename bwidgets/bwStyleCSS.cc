#include "bwAbstractButton.h"
#include "bwPainter.h"

#include "bwStyleCSS.h"

#include "bwPanel.h"
#include "bwTextBox.h"  // XXX Ugly

namespace bWidgets {

void (*bwStyleCSS::polish_cb)(class bwWidget&) = nullptr;

bwStyleCSS::bwStyleCSS() : bwStyle(bwStyle::TypeID::CLASSIC_CSS)
{
}

void bwStyleCSS::setWidgetStyle(bwWidget& widget)
{
  std::optional<std::reference_wrapper<bwWidgetBaseStyle>> base_style;

  polish(widget);

  if (auto* button = widget_cast<bwAbstractButton>(widget)) {
    button->base_style.roundbox_corners = button->rounded_corners;
    base_style = button->base_style;
  }
  else if (auto* container = widget_cast<bwContainerWidget>(widget)) {
    container->base_style.roundbox_corners = RoundboxCorner::ALL;
    base_style = container->base_style;
  }
  else if (auto* text_box = widget_cast<bwTextBox>(widget)) {
    text_box->base_style.roundbox_corners =
        RoundboxCorner::ALL;  // XXX Incorrect, should set this in layout.
    base_style = text_box->base_style;
  }
  else {
    //		base_style->roundbox_corners = RoundboxCorner::ALL;
  }
}

void bwStyleCSS::polish(bwWidget& widget)
{
  if (polish_cb) {
    polish_cb(widget);
  }
}

}  // namespace bWidgets
