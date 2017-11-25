#pragma once

#include "bwRadioButton.h"

#include "Stage.h"

namespace bWidgetsDemo {

class DefaultStage : public Stage
{
	friend class ScaleSetter;

public:
	DefaultStage(unsigned int mask_width, unsigned int mask_height);

private:
	void addStyleSelector(class LayoutItem& parent_layout);
	void addFakeSpacer(class LayoutItem& layout);

	const unsigned int padding = 10;
};

} // namespace bWidgetsDemo
