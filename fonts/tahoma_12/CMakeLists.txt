project(tahoma_12)

set(BIN_SOURCES
    tahoma_12.cpp
)

set(LIB_HEADERS
    public/tahoma_12.h
)
 
mark3_add_library(tahoma_12 ${BIN_SOURCES})

target_include_directories(tahoma_12 
    PUBLIC
        public
)

target_link_libraries(tahoma_12
    graphics
)
