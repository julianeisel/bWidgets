#include "Label.h"
#include "Painter.h"
#include "Style.h"

using namespace bWidgets;

Label::Label(
        const std::string& text,
        unsigned int position_x, unsigned int position_y,
        unsigned int width, unsigned int height) :
    Widget(Widget::WIDGET_TYPE_LABEL,
           Rectangle<unsigned int>(position_x, position_x + width, position_y, position_y + height)),
    text(text)
{
	
}

void Label::onClick()
{
	/* nothing */
}
void Label::mouseEnter()
{
	/* nothing */
}
void Label::mouseLeave()
{
	/* nothing */
}

void Label::draw(Style& style)
{
	Style::WidgetStyle& widget_style = style.widget_style;
	Painter painter;

	style.setWidgetStyle(*this);

	painter.setActiveColor(widget_style.text_color);
	painter.drawText(text, rectangle, Painter::text_draw_arg);
}
