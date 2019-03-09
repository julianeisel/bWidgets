#include "bwPainter.h"
#include "bwStyle.h"

#include "bwAbstractButton.h"

using namespace bWidgets;


bwAbstractButton::bwAbstractButton(
        std::string text,
        const WidgetType type, const std::string& identifier,
        const unsigned int width_hint, const unsigned int height_hint) :
    bwWidget(type, identifier, width_hint, height_hint),
    rounded_corners(RoundboxCorner::ALL),
    text(std::move(text))
{
	initialize();
}

void bwAbstractButton::draw(bwStyle& style)
{
	style.setWidgetStyle(*this);

	const bwGradient gradient{
	        base_style.backgroundColor(),
	        base_style.shadeTop(), base_style.shadeBottom()
	};
	bwPainter painter;

	painter.drawRoundboxWidgetBase(base_style, style, rectangle, gradient, base_style.corner_radius);

	// Text
	painter.setContentMask(rectangle);
	painter.setActiveColor(base_style.textColor());
	painter.drawTextAndIcon(text, getIcon(), rectangle, base_style.text_alignment, style.dpi_fac);
}

void bwAbstractButton::registerProperties()
{
	base_style.registerProperties(style_properties);
}

void bwAbstractButton::mousePressEvent(
        const bwWidget::MouseButton button,
        const bwPoint& /*location*/)
{
	if (button == MOUSE_BUTTON_LEFT) {
		state = STATE_SUNKEN;
	}
}

void bwAbstractButton::mouseReleaseEvent(
        const bwWidget::MouseButton button,
        const bwPoint& /*location*/)
{
	if ((button == MOUSE_BUTTON_LEFT) && (state == STATE_SUNKEN)) {
		state = STATE_NORMAL;
	}
}

void bwAbstractButton::onMouseEnter()
{
	if (state == STATE_NORMAL) {
		state = STATE_HIGHLIGHTED;
	}
}

void bwAbstractButton::onMouseLeave()
{
	if (state == STATE_HIGHLIGHTED) {
		state = STATE_NORMAL;
	}
}

const std::string* bwAbstractButton::getLabel() const
{
	return &text;
}

const bwIconInterface *bwAbstractButton::getIcon() const
{
	return nullptr;
}

void bwAbstractButton::apply()
{
	if (apply_functor) {
		(*apply_functor)();
	}
}
