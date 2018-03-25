#pragma once

#include "bwRadioButton.h"

#include "Stage.h"

namespace bWidgetsDemo {

class DefaultStage : public Stage
{
	friend class ScaleSetter;
	friend class StyleSetter;
	friend class UseCSSVersionToggleSetter;

public:
	DefaultStage(unsigned int mask_width, unsigned int mask_height);

private:
	void activateStyleID(bwStyle::StyleTypeID) override;
	void addStyleSelector(class LayoutItem& parent_layout);
	void addFakeSpacer(class LayoutItem& layout);

	void useStyleCSSVersionSet(const bool use_css_version);

	const unsigned int padding = 10;
};

} // namespace bWidgetsDemo
