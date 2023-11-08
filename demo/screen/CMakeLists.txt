# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, see <http://www.gnu.org/licenses/>.
#
# Original work Copyright (c) 2018 Julian Eisel
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	${FREETYPE_INCLUDE_DIRS}
	${GPU_INC}
	${PNG_PNG_INCLUDE_DIR}
	..
	../extern/katana-parser/src
	../extern/gawain
	../gpu
	../resources
	../stylesheet
	../utils
	../../bwidgets/styles
)

set(SRC
	DefaultStage.cc
	DefaultStageRNAFunctor.cc
	Font.cc
	GawainPaintEngine.cc
	IconMap.cc
	Layout.cc
	Stage.cc

	DummyFunctors.hh
	DefaultStage.h
	RNABinding.h
	DefaultStageRNAFunctor.h
	Font.h
	GawainPaintEngine.h
	IconMap.h
	Layout.h
	Stage.h
)

set(LIB
	${FREETYPE_LIBRARY}
	${ZLIB_LIBRARY}
	${PNG_LIBRARY}
	glfw
	bWidgets

	bwd_stylesheet
)
add_definitions(-DGLEW_STATIC)

add_library(bwd_screen)
target_sources(bwd_screen PRIVATE ${SRC})
include_directories(${INC})
target_link_libraries(bwd_screen ${LIB})
