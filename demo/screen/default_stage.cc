#include "PushButton.h"
#include "Stage.h"

#include "default_stage.h"

using namespace bWidgetDemo;


Stage* SCR_default_stage_add()
{
	Stage* stage = new Stage();
	const int but_width = 100;
	const int but_height = 20;
	const int pad = 10;

	for (int i = 0; i < 10; i++) {
		const int xmin = pad + (i * (pad + but_width));
		const int ymin = pad;
		bWidgets::PushButton* widget = new bWidgets::PushButton("Widget " + std::to_string(i),
		                                                                        xmin, ymin, but_width, but_height);
		stage->widgetAdd(widget);
	}

	return stage;
}
