project(arial_12)

set(BIN_SOURCES
    arial_12.cpp
)

set(LIB_HEADERS
    public/arial_12.h
)
 
mark3_add_library(arial_12 ${BIN_SOURCES})

target_include_directories(arial_12 
    PUBLIC
        public
)

target_link_libraries(arial_12
    graphics
)
