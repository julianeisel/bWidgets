#include "Font.h"
#include "Layout.h"
#include "Stage.h"

#include "bwLabel.h"
#include "bwPushButton.h"
#include "bwRadioButton.h"
#include "bwStyle.h"
#include "bwStyleManager.h"
#include "bwTextBox.h"

#include "DefaultStage.h"

using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose


#define BUTTON_HEIGHT 20

DefaultStage::DefaultStage(unsigned int width, unsigned int height) :
    Stage(width, height)
{
	ColumnLayout* col = new ColumnLayout(true);

	addStyleSelector(*layout);

	layout->addLayout(col);
	col->addWidget(new bwPushButton("Translate", 0, 0, 0, BUTTON_HEIGHT));
	col->addWidget(new bwPushButton("Rotate", 0, 0, 0, BUTTON_HEIGHT));
	col->addWidget(new bwPushButton("Scale", 0, 0, 0, BUTTON_HEIGHT));

	layout->addWidget(new bwPushButton("Mirror", 0, 0, 0, BUTTON_HEIGHT));
	// Just some extra space. No spacer widgets yet.
	layout->addWidget(new bwLabel("", 0, 0, 0, 7));

	bwTextBox* text_box = new bwTextBox(0, 0, 0, BUTTON_HEIGHT);
	text_box->setText("Some Text...");
	layout->addWidget(text_box);
}

void DefaultStage::addStyleSelector(LayoutItem& parent_layout)
{
	RowLayout* row_layout = new RowLayout(true, &parent_layout);

	row_layout->addWidget(new bwLabel("Style :", 0, 0, 0, BUTTON_HEIGHT));

	for (bwStyle::StyleType type : bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
		bwRadioButton* style_button = new bwRadioButton(type.name, 0, 0, 0, BUTTON_HEIGHT);

		style_button->valueID = type.type_id;
		style_button->custom_data = this;
		style_button->apply = StyleApplyButtonCb;

		if (type.type_id == style->type_id) {
			style_button->state = bwAbstractButton::STATE_SUNKEN;
		}

		row_layout->addWidget(style_button);
	}
}

bool DefaultStage::StyleButtonsUpdateCb(bwWidget& widget_iter, void* custom_data)
{
	bwRadioButton* lookup_radio_button = (bwRadioButton*)custom_data;

	if ((&widget_iter != lookup_radio_button) && (widget_iter.type == bwWidget::WIDGET_TYPE_RADIO_BUTTON)) {
		bwRadioButton* radio_iter = widget_cast<bwRadioButton*>(&widget_iter);
		if (radio_iter->custom_data == lookup_radio_button->custom_data) {
			if (radio_iter->valueID == lookup_radio_button->valueID) {
				radio_iter->state = bwAbstractButton::STATE_SUNKEN;
			}
			else {
				radio_iter->state = bwAbstractButton::STATE_NORMAL;
			}
		}
	}

	return true;
}

void DefaultStage::StyleApplyButtonCb(bwWidget& widget)
{
	bwRadioButton& radio_but = *widget_cast<bwRadioButton*>(&widget);
	DefaultStage* stage = reinterpret_cast<DefaultStage*>(radio_but.custom_data);

	stage->activateStyleID((bwStyle::StyleTypeID)radio_but.valueID);

	// Deactivate other style radio buttons
	stage->layout->iterateWidgets(StyleButtonsUpdateCb, &radio_but);
}
