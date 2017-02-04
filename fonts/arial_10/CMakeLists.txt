project(arial_10)

set(BIN_SOURCES
    arial_10.cpp
)

set(LIB_HEADERS
    public/arial_10.h
)
 
mark3_add_library(arial_10 ${BIN_SOURCES})

target_include_directories(arial_10 
    PUBLIC
        public
)

target_link_libraries(arial_10
    graphics
)
