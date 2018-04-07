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
)

set(SRC
	src/foundation.c
	src/katana.lex.c
	src/katana.tab.c
	src/parser.c
	src/selector.c
	src/tokenizer.c

	src/foundation.h
	src/katana.h
	src/katana.lex.h
	src/katana.tab.h
	src/parser.h
	src/selector.h
	src/tokenizer.h
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	list(APPEND INC visualc/include)
endif()

include_directories(${INC})
add_library(extern_katana ${SRC})
