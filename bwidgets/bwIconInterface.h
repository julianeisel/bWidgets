#pragma once

namespace bWidgets {

/**
 * \brief Interface to reference icons from within bWidgets.
 *
 * Using this interface, bWidgets can reference icon objects of which it
 * doesn't know any implementation details.
 *
 * To get icon drawing to work you have to:
 * * implement this interface as some class for storing icon data (e.g. a
 *   bitmap wrapper),
 * * implement the \link bWidgets::bwPaintEngine::drawIcon
 *   bwPaintEngine::drawIcon() function for drawing the icon data,
 * * call the `setIcon` function of a widget (if available) to set the icon
 *   reference.
 *
 * \note Managing icons, including their memory, has to be done outside of
 *       bWidgets. At no point does bWidgets take ownership over icon data.
 */
class bwIconInterface {
 public:
  constexpr static float ICON_DEFAULT_SIZE = 16;
  virtual bool isValid() const = 0;
};

}  // namespace bWidgets
