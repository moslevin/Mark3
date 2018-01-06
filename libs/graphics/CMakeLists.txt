project(graphics)

set(LIB_SOURCES
    bitstream.cpp
    graphics.cpp
    tiles.cpp
)

set(LIB_HEADERS
    public/bitstream.h
    public/colordepth.h
    public/colorspace.h
    public/draw.h
    public/font.h
    public/fontport.h
    public/graphics.h
    public/tiles.h
)

mark3_add_library(graphics ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(graphics 
    PUBLIC
        public        
)

target_link_libraries(graphics 
    driver
    mark3
)
