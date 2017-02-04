project(courier_10)

set(BIN_SOURCES
    courier_10.cpp
)

set(LIB_HEADERS
    public/courier_10.h
)
 
mark3_add_library(courier_10 ${BIN_SOURCES})

target_include_directories(courier_10 
    PUBLIC
        public
)

target_link_libraries(courier_10
    graphics
)
