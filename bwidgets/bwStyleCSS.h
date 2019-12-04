#pragma once

#include "bwStyle.h"

namespace bWidgets {

/**
 * \brief Style-class for CSS driven Styles.
 *
 * How CSS is applied to styles may have to be reworked a bit. For now it just
 * mattered to get something working (that is, making CSS control a style).
 * Maybe every style should have the ability to be manipulated via CSS? We
 * could for example store a CSS-Tree structure in each style that has priority
 * over hardcoded style.
 * It's also important to keep custom styles in mind. In the end, users should
 * be able to 'install' a new style via a CSS file.
 */
class bwStyleCSS : public bwStyle {
 public:
  bwStyleCSS();

  void setWidgetStyle(class bwWidget& widget);
  void polish(class bwWidget& widget);

  static void (*polish_cb)(class bwWidget& widget);
};

}  // namespace bWidgets
