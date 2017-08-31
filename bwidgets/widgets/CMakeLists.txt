set(INC
)

set(SRC
	bwAbstractButton.cc
	bwLabel.cc
	bwNumberSlider.cc
	bwPushButton.cc
	bwRadioButton.cc
	bwTextBox.cc
	bwWidget.cc

	bwAbstractButton.h
	bwLabel.h
	bwNumberSlider.h
	bwPushButton.h
	bwRadioButton.h
	bwTextBox.h
	bwWidget.h
)

add_library(bw_widgets ${SRC})
include_directories(${INC})
