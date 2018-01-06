project(drvUOLED)

set(LIB_SOURCES
    graphics_uoled.cpp
)

set(LIB_HEADERS
    public/graphics_uoled.h
    public/uoled_defines.h
)

mark3_add_library(drvUOLED ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(drvUOLED
    PUBLIC
        public        
    )
    
target_link_libraries(drvUOLED
    driver
    mark3
    graphics
)
