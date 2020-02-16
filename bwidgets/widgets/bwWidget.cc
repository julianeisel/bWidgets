#include "bwWidget.h"

using namespace bWidgets;

bwWidget::bwWidget(const Type type,
                   const std::string& identifier,
                   const unsigned int width_hint,
                   const unsigned int height_hint)
    : type(type),
      state(State::NORMAL),
      rectangle(0, 0, 0, 0),
      width_hint(width_hint),
      height_hint(height_hint),
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
