set(INC
)

set(SRC
	bwAbstractButton.cc
	bwCheckbox.cc
	bwContainerWidget.cc
	bwLabel.cc
	bwNumberSlider.cc
	bwPanel.cc
	bwPushButton.cc
	bwRadioButton.cc
	bwScrollBar.cc
	bwScrollView.cc
	bwTextBox.cc
	bwWidget.cc

	builtin_widgets.h
	bwAbstractButton.h
	bwCheckbox.h
	bwContainerWidget.h
	bwLabel.h
	bwNumberSlider.h
	bwPanel.h
	bwPushButton.h
	bwRadioButton.h
	bwScrollBar.h
	bwScrollView.h
	bwTextBox.h
	bwWidget.h
)

set(LIB
	bWidgets
)

add_library(bw_widgets)
target_sources(bw_widgets PRIVATE ${SRC})
include_directories(${INC})
target_link_libraries(bw_widgets ${LIB})
