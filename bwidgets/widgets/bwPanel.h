#pragma once

#include "bwWidget.h"


namespace bWidgets {

class bwPanel : public bwWidget
{
public:
	bwPanel(
	        const std::string& label,
	        unsigned int header_height_hint);

	void draw(class bwStyle& style) const;

	void mousePressEvent(
	        const MouseButton button,
	        const bwPoint& location) override;

	bool isCoordinateInsideHeader(const bwPoint &point) const;
	unsigned int getHeaderHeightHint() const;
	unsigned int header_height;

	enum {
		PANEL_OPEN,
		PANEL_CLOSED,
	} panel_state{PANEL_OPEN};

private:
	void drawHeader(class bwStyle& style) const;
	bwRectanglePixel getHeaderRectangle() const;

	std::string label;
};

} // namespace bWidgets
