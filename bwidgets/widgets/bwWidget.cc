#include "bwStyle.h"

#include "bwWidget.h"

namespace bWidgets {

bwWidget::bwWidget(const std::string& identifier,
                   std::optional<unsigned int> width_hint,
                   std::optional<unsigned int> height_hint)
    : state(State::NORMAL),
      rectangle(0, 0, 0, 0),
      width_hint(width_hint.value_or(bwStyle::s_default_widget_size_hint)),
      height_hint(height_hint.value_or(bwStyle::s_default_widget_size_hint)),
      identifier(identifier)
{
}

bool bwWidget::isCoordinateInside(const bwPoint& point) const
{
  return rectangle.isCoordinateInside(point.x, point.y);
}

const std::string& bwWidget::getIdentifier() const
{
  return identifier;
}

const std::string* bwWidget::getLabel() const
{
  return nullptr;
}

bool bwWidget::canAlign() const
{
  return false;
}

/**
 * \brief Additional initialization that can't be done in #bwWidget constructor.
 */
void bwWidget::initialize()
{
  /* Has to be done in init, can't be called from bwWidget constructor (wouldn't call overwritten
   * function). */
  registerProperties();
}

void bwWidget::registerProperties()
{
}

}  // namespace bWidgets
