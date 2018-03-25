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

	style.setWidgetStyle(*this);

	painter.setContentMask(rectangle);

	painter.setActiveColor(text_color);
	painter.drawText(text, rectangle, TEXT_ALIGN_LEFT);
}

void bwLabel::registerProperties()
{
	style_properties.addColor("color", text_color);
}
