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

# No source files here, causing error on clang.
if(False)
	set(INC
	)

	set(SRC
		FixedNum.h
	)

	add_library(bwd_utils)
	target_sources(bwd_utils PRIVATE ${SRC})
	include_directories(${INC})
	set_target_properties(bwd_utils PROPERTIES LINKER_LANGUAGE CXX)
endif()
