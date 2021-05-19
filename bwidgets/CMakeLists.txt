set(INC
	.
	generics
	screen_graph
	styling
	styling/styles
	utils
	widgets
)

set(SRC
	bwEvent.cc
	bwEventDispatcher.cc
	bwPainter.cc
	screen_graph/Builder.cc
	screen_graph/Constructor.cc
	screen_graph/Drawer.cc
	screen_graph/EventHandler.cc
	screen_graph/Iterators.cc
	screen_graph/PersistentRef.cc
	styling/bwStyle.cc
	styling/bwStyleCSS.cc
	styling/bwStyleManager.cc
	styling/bwStyleProperties.cc
	styling/bwWidgetBaseStyle.cc
	styling/styles/bwStyleClassic.cc
	styling/styles/bwStyleFlatGrey.cc
	styling/styles/bwStyleFlatDark.cc
	styling/styles/bwStyleFlatLight.cc

	bwContext.h
	bwEvent.h
	bwEventDispatcher.h
	bwIconInterface.h
	bwLayoutInterface.h
	bwPaintEngine.h
	bwPainter.h
	screen_graph/Builder.h
	screen_graph/Constructor.cc
	screen_graph/Drawer.h
	screen_graph/EventHandler.h
	screen_graph/Iterators.h
	screen_graph/Node.h
	screen_graph/ScreenGraph.h
	screen_graph/PersistentRef.h
	styling/bwStyle.h
	styling/bwStyleCSS.h
	styling/bwStyleManager.h
	styling/bwStyleProperties.h
	styling/bwWidgetBaseStyle.h
	styling/styles/bwStyleClassic.h
	styling/styles/bwStyleFlatGrey.h
	styling/styles/bwStyleFlatDark.h
	styling/styles/bwStyleFlatLight.h
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
