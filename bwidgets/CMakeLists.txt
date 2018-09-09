set(INC
	.
	generics
	styles
	style_properties
	utils
	widgets
)

set(SRC
	bWidgets.cc
	bwPainter.cc
	bwStyle.cc
	bwStyleCSS.cc
	bwStyleManager.cc
	bwWidgetBaseStyle.cc
	styles/bwStyleClassic.cc
	styles/bwStyleFlatGrey.cc
	styles/bwStyleFlatDark.cc
	styles/bwStyleFlatLight.cc

	bWidgets.h
	bwIconInterface.h
	bwPaintEngine.h
	bwPainter.h
	bwStyle.h
	bwStyleCSS.h
	bwStyleManager.h
	bwWidgetBaseStyle.h
	styles/bwStyleClassic.h
	styles/bwStyleFlatGrey.h
	styles/bwStyleFlatDark.h
	styles/bwStyleFlatLight.h
)

set(LIB
	bw_style_properties
	bw_generics
	bw_utils
	bw_widgets
)

add_library(bWidgets ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})

add_subdirectory(generics)
add_subdirectory(style_properties)
add_subdirectory(utils)
add_subdirectory(widgets)
