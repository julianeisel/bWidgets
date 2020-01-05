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

add_library(bw_widgets ${SRC})
include_directories(${INC})
target_link_libraries(bw_widgets ${LIB})
