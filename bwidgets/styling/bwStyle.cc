#include <cassert>

#include "bwStyle.h"

namespace bWidgets {

unsigned int bwStyle::s_default_widget_size_hint = 20;

bwStyle::bwStyle(TypeID type_id) : type_id(type_id)
{
}

void bwStyle::polish(class bwWidget&)
{
  /* Nothing by default. */
}

}  // namespace bWidgets
