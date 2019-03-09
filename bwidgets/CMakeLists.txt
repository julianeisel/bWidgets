set(INC
	.
	generics
	styles
	utils
	widgets
)

set(SRC
	bwPainter.cc
	bwStyle.cc
	bwStyleCSS.cc
	bwStyleManager.cc
	bwStyleProperties.cc
	bwWidgetBaseStyle.cc
	screen_graph/Builder.cc
	screen_graph/EventHandler.cc
	screen_graph/Iterators.cc
	styles/bwStyleClassic.cc
	styles/bwStyleFlatGrey.cc
	styles/bwStyleFlatDark.cc
	styles/bwStyleFlatLight.cc

	bwIconInterface.h
	bwLayoutInterface.h
	bwPaintEngine.h
	bwPainter.h
	bwStyle.h
	bwStyleCSS.h
	bwStyleManager.h
	bwStyleProperties.h
	bwWidgetBaseStyle.h
	screen_graph/Builder.h
	screen_graph/EventHandler.h
	screen_graph/Iterators.h
	screen_graph/Node.h
	styles/bwStyleClassic.h
	styles/bwStyleFlatGrey.h
	styles/bwStyleFlatDark.h
	styles/bwStyleFlatLight.h
)

set(LIB
	bw_generics
	bw_widgets
)

add_library(bWidgets ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})

add_subdirectory(generics)
add_subdirectory(utils)
add_subdirectory(widgets)
