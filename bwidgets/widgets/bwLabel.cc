#include <math.h>

#include "bwPainter.h"
#include "bwStyle.h"

#include "bwLabel.h"

using namespace bWidgets;


bwLabel::bwLabel(
        const std::string& text,
        unsigned int width_hint, unsigned int height_hint) :
    bwWidget(bwWidget::WIDGET_TYPE_LABEL, "bwLabel", width_hint, height_hint),
    text(text)
{
	initialize();
}

void bwLabel::draw(bwStyle& style)
{
	bwPainter painter;
	bwRectanglePixel& icon_rect{rectangle};
	bwRectanglePixel& text_rect{rectangle};

	style.setWidgetStyle(*this);

	if (icon) {
		const float icon_size = std::round(bwIconInterface::ICON_DEFAULT_SIZE * style.dpi_fac);
		icon_rect.xmax = icon_rect.xmin + icon_size;
		icon_rect.ymax = icon_rect.ymin + icon_size;
		painter.drawIcon(**icon, icon_rect);
		text_rect.xmin = icon_rect.xmax;
	}

	painter.setContentMask(rectangle);

	painter.setActiveColor(text_color);
	painter.drawText(text, rectangle, TEXT_ALIGN_LEFT);
}

void bwLabel::registerProperties()
{
	style_properties.addColor("color", text_color);
}

bwLabel& bwLabel::setIcon(const bwIconInterface& icon_interface)
{
	icon = &icon_interface;
	return *this;
}
