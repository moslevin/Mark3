project(graphics_flavr)

set(LIB_SOURCES
    graphics_flavr.cpp
)

set(LIB_HEADERS
    public/graphics_flavr.h
)

mark3_add_library(graphics_flavr ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(graphics_flavr
    PUBLIC
        public        
    )
    
target_link_libraries(graphics_flavr
    driver
    graphics
    mark3
)
