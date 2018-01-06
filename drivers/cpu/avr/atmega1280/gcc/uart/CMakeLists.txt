project(atmegauart)

set(LIB_SOURCES
    drvATMegaUART.cpp
)

set(LIB_HEADERS
    public/drvATMegaUART.h
)

mark3_add_library(atmegauart ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(atmegauart
    PUBLIC
        public
    )

target_link_libraries(atmegauart
    driver
    mark3
    uart
)
