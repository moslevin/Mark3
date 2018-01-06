project(graphics_st7735)

set(LIB_SOURCES
    graphics_st7735.cpp
)

set(LIB_HEADERS
    public/graphics_st7735.h
)

mark3_add_library(graphics_st7735 ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(graphics_st7735
    PUBLIC
        public        
    )
    
target_link_libraries(graphics_st7735
    driver
    graphics
    mark3
)
