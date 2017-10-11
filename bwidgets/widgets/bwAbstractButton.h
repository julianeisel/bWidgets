#pragma once

#include <string>

#include "bwWidget.h"

namespace bWidgets {

/**
 * \brief Base class for button like widgets.
 */
class bwAbstractButton : public bwWidget
{
public:
	virtual void draw(class bwStyle &style) const override;

	virtual void mousePressEvent(
	        const MouseButton button,
	        const bwPoint& location) override;
	virtual void mouseReleaseEvent(
	        const MouseButton button,
	        const bwPoint& location) override;
	virtual void mouseEnter() override;
	virtual void mouseLeave() override;

	void (*apply)(bwWidget& widget);

	unsigned int rounded_corners;

protected:
	// Protected constructor to force calling through inherited class (pseudo abstract).
	bwAbstractButton(
	        const std::string& text,
	        const WidgetType type,
	        const unsigned int width_hint = 0, const unsigned int height_hint = 0);

	const std::string text;
};

} // namespace bWidgets
