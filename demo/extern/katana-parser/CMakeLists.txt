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