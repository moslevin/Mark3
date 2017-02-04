project(sw_spi)

set(LIB_SOURCES
    sw_spi.cpp
)

set(LIB_HEADERS
    public/sw_spi.h
)

mark3_add_library(sw_spi ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(sw_spi
    PUBLIC
        public        
    )
    
target_link_libraries(sw_spi
    mark3
)