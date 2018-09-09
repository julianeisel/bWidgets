#pragma once

#include <memory>
#include <string>

#include "bwUtil.h"
#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"


namespace bWidgets {

/**
 * \brief Base class for button like widgets.
 */
class bwAbstractButton : public bwWidget
{
public:
	virtual void draw(class bwStyle &style) override;
	virtual void registerProperties() override;

	virtual void mousePressEvent(
	        const MouseButton button,
	        const bwPoint& location) override;
	virtual void mouseReleaseEvent(
	        const MouseButton button,
	        const bwPoint& location) override;
	virtual void mouseEnter() override;
	virtual void mouseLeave() override;

	/**
	 * Function object called when applying changes to widget.
	 */
	bwPointer<bwFunctorInterface> apply_functor{nullptr};

	unsigned int rounded_corners;

protected:
	// Protected constructor to force calling through inherited class (pseudo abstract).
	bwAbstractButton(
	        const std::string& text,
	        const WidgetType type, const std::string& identifier,
	        const unsigned int width_hint = 0, const unsigned int height_hint = 0);
	void apply();

	const std::string text;

public: bwWidgetBaseStyle base_style; // XXX public for setWidgetStyle. Should only be temporarily needed.
};

} // namespace bWidgets
