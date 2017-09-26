#include "bwStyle.h"

#include "bwLabel.h"

using namespace bWidgets;


bwLabel::bwLabel(
        const std::string& text,
        unsigned int width_hint, unsigned int height_hint) :
    bwWidget(bwWidget::WIDGET_TYPE_LABEL, width_hint, height_hint),
    text(text)
{
	
}

void bwLabel::draw(bwStyle& style) const
{
	bwWidgetStyle& widget_style = style.widget_styles[type];
	bwPainter painter;

	style.setWidgetStyle(*this);
	painter.setContentMask(rectangle);

	painter.setActiveColor(widget_style.textColor(state));
	painter.drawText(text, rectangle, widget_style.text_alignment);
}
