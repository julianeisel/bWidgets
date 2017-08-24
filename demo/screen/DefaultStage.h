#pragma once

#include "bwRadioButton.h"

#include "Stage.h"

namespace bWidgetsDemo {

class DefaultStage : public Stage
{
public:
	DefaultStage(unsigned int width, unsigned int height);

private:
	void addStyleSelector(class LayoutItem& parent_layout);

	static void StyleApplyButtonCb(
	        bWidgets::bwWidget& widget);
	static bool StyleButtonsUpdateCb(
	        bWidgets::bwWidget& widget_iter,
	        void* custom_data);

	const unsigned int padding = 10;
};

} // namespace bWidgetsDemo
