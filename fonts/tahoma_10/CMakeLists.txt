project(tahoma_10)

set(BIN_SOURCES
    tahoma_10.cpp
)

set(LIB_HEADERS
    public/tahoma_10.h
)
 
mark3_add_library(tahoma_10 ${BIN_SOURCES})

target_include_directories(tahoma_10 
    PUBLIC
        public
)

target_link_libraries(tahoma_10
    graphics
)
