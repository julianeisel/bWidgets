#pragma once

#include "bwPaintEngine.h"

namespace bWidgetsDemo {

class GawainPaintEngine : public bWidgets::bwPaintEngine
{
public:
	GawainPaintEngine(class Font&);

	void setupViewport(
	        const bWidgets::bwRectanglePixel&,
	        const class bWidgets::bwColor&) override;

	void drawPolygon(
	        const class bWidgets::bwPainter&,
	        const class bWidgets::bwPolygon&) override;
	void drawText(
	        const class bWidgets::bwPainter &,
	        const std::string&,
	        const bWidgets::bwRectanglePixel&,
	        const bWidgets::TextAlignment) override;

private:
	class Font& font;
};

} // namespace bWidgets
