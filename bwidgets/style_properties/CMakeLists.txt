set(INC
	../generics
)

set(SRC
	bwStyleProperties.cc
	bwStylePropertyType.cc
	bwStyleStruct.cc
	bwStyleStructTypeMap.cc
	css_background.cc
	css_border.cc
	css_color.cc
	css_custom_props.cc

	bwStyleProperties.h
	bwStylePropertyType.h
	bwStyleStruct.h
	bwStyleStructTypeMap.h
	css_background.h
	css_border.h
	css_color.h
	css_custom_props.h
)

set(LIB
)

add_library(bw_style_properties ${SRC})
include_directories(${INC})
target_link_libraries(bw_style_properties ${LIB})
