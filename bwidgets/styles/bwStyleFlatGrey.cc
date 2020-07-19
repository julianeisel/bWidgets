#include <cassert>

#include "bwAbstractButton.h"
#include "bwCheckbox.h"
#include "bwNumberSlider.h"
#include "bwPainter.h"
#include "bwPanel.h"
#include "bwPushButton.h"
#include "bwRadioButton.h"
#include "bwScrollBar.h"
#include "bwScrollView.h"
#include "bwTextBox.h"

#include "bwStyleFlatGrey.h"

namespace bWidgets {

static void widget_base_style_checkbox_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 0.27451f;
  r_base_style.text_color = 0.0f;
  r_base_style.border_color = 0.1f;
  r_base_style.decoration_color = 1.0f;
  r_base_style.corner_radius = 7.0f;

  switch (widget.getState()) {
    case bwWidget::State::HIGHLIGHTED:
      r_base_style.background_color.shade(0.06f);
      break;
    case bwWidget::State::SUNKEN:
      r_base_style.text_color = 1.0f;
      std::swap(r_base_style.shade_top, r_base_style.shade_bottom);
      break;
    default:
      break;
  }
}
static void widget_base_style_number_slider_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 0.6f;
  r_base_style.text_color = 0.0f;
  r_base_style.border_color = 0.2f;
  r_base_style.decoration_color = 0.353f;
  r_base_style.corner_radius = 4.0f;

  switch (widget.getState()) {
    case bwWidget::State::HIGHLIGHTED:
      r_base_style.background_color.shade(0.06f);
      break;
    case bwWidget::State::SUNKEN:
      r_base_style.text_color = 1.0f;
      break;
    default:
      break;
  }
}
static void widget_base_style_push_button_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 0.6f;
  r_base_style.text_color = 0.0f;
  r_base_style.border_color = 0.3f;
  r_base_style.corner_radius = 8.0f;

  switch (widget.getState()) {
    case bwWidget::State::HIGHLIGHTED:
      r_base_style.background_color.shade(0.06f);
      break;
    case bwWidget::State::SUNKEN:
      r_base_style.background_color = 0.353f;
      r_base_style.text_color = 1.0f;
      std::swap(r_base_style.shade_top, r_base_style.shade_bottom);
      break;
    default:
      break;
  }
}
static void widget_base_style_radio_button_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 0.27451f;
  r_base_style.text_color = 1.0f;
  r_base_style.border_color = 0.1f;
  r_base_style.text_alignment = TextAlignment::CENTER;
  r_base_style.corner_radius = 6.0f;

  switch (widget.getState()) {
    case bwWidget::State::HIGHLIGHTED:
      r_base_style.background_color.shade(0.06f);
      break;
    case bwWidget::State::SUNKEN:
      r_base_style.background_color = bwColor(0.337255f, 0.501961f, 0.760784f);
      r_base_style.text_color = 0.0f;
      std::swap(r_base_style.shade_top, r_base_style.shade_bottom);
      break;
    default:
      break;
  }
}
static void widget_base_style_scroll_bar_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = bwColor(80u, 180u);
  r_base_style.text_color = 0.0f;
  r_base_style.border_color = bwColor(50u);
  r_base_style.decoration_color = bwColor(128u);
  r_base_style.corner_radius = 6.5f;

  switch (widget.getState()) {
    case bwWidget::State::HIGHLIGHTED:
      r_base_style.background_color.shade(0.06f);
      break;
    case bwWidget::State::SUNKEN:
      r_base_style.background_color = bwColor(100u, 180u);
      r_base_style.text_color = 1.0f;
      break;
    default:
      break;
  }
}
static void widget_base_style_text_box_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 0.6f;
  r_base_style.text_color = 0.0f;
  r_base_style.border_color = 0.2f;
  r_base_style.decoration_color = 0.353f;
  r_base_style.corner_radius = 4.0f;

  switch (widget.getState()) {
    case bwWidget::State::HIGHLIGHTED:
      r_base_style.background_color.shade(0.06f);
      break;
    case bwWidget::State::SUNKEN:
      r_base_style.text_color = 1.0f;
      break;
    default:
      break;
  }
}
static void widget_base_style_panel_set(bwWidget& /*widget*/, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 134u;
  r_base_style.border_color = 134u;
}

static void widget_base_style_scrollview_set(bwWidget&, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.background_color = 114u;
  r_base_style.border_color = 114u;
}

static void widget_style_properties_set_to_default(bwWidget& widget)
{
  for (auto& property : widget.style_properties) {
    property->setValueToDefault();
  }
}

static void widget_base_style_set(bwWidget& widget, bwWidgetBaseStyle& r_base_style)
{
  r_base_style.shade_top = r_base_style.shade_bottom = 0.0f;

  /* Anti-pattern/code-smell: This kind of widget_cast<>() usage to eventually find the correct
   * type is rather inefficient (compared to an enum type checked in a switch).
   * This isn't performance critical code, so it's fine-ish, even if ugly. More importantly, this
   * way of applying styles isn't meant as permanent solution anyway. */
  if (widget_cast<bwCheckbox>(widget)) {
    widget_base_style_checkbox_set(widget, r_base_style);
  }
  else if (widget_cast<bwNumberSlider>(widget)) {
    widget_base_style_number_slider_set(widget, r_base_style);
  }
  else if (widget_cast<bwPushButton>(widget)) {
    widget_base_style_push_button_set(widget, r_base_style);
  }
  else if (widget_cast<bwRadioButton>(widget)) {
    widget_base_style_radio_button_set(widget, r_base_style);
  }
  else if (widget_cast<bwScrollBar>(widget)) {
    widget_base_style_scroll_bar_set(widget, r_base_style);
  }
  else if (widget_cast<bwTextBox>(widget)) {
    widget_base_style_text_box_set(widget, r_base_style);
  }
  else if (widget_cast<bwPanel>(widget)) {
    widget_base_style_panel_set(widget, r_base_style);
  }
  else if (widget_cast<bwScrollView>(widget)) {
    widget_base_style_scrollview_set(widget, r_base_style);
  }
}

bwStyleFlat::bwStyleFlat() : bwStyle(TypeID::FLAT_GREY)
{
}

void bwStyleFlat::setWidgetStyle(bwWidget& widget)
{
  bwWidgetBaseStyle* base_style = nullptr;

  polish(widget);

  if (auto* button = widget_cast<bwAbstractButton>(widget)) {
    button->base_style.roundbox_corners = button->rounded_corners;
    base_style = &button->base_style;
  }
  else if (auto* text_box = widget_cast<bwTextBox>(widget)) {
    text_box->base_style.roundbox_corners =
        RoundboxCorner::ALL;  // XXX Incorrect, should set this in layout.
    base_style = &text_box->base_style;
  }
  else if (auto* container = widget_cast<bwContainerWidget>(widget)) {
    container->base_style.roundbox_corners = RoundboxCorner::ALL;
    base_style = &container->base_style;
  }
  else {
    //		base_style->roundbox_corners = RoundboxCorner::ALL;
  }

  widget_style_properties_set_to_default(widget);

  if (base_style) {
    widget_base_style_set(widget, *base_style);
  }
}

}  // namespace bWidgets
