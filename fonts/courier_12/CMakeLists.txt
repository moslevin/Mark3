project(courier_12)

set(BIN_SOURCES
    courier_12.cpp
)

set(LIB_HEADERS
    public/courier_12.h
)
 
mark3_add_library(courier_12 ${BIN_SOURCES})

target_include_directories(courier_12 
    PUBLIC
        public
)

target_link_libraries(courier_12
    graphics
)
