project(sw_i2c)

set(LIB_SOURCES
    sw_i2c.cpp
)

set(LIB_HEADERS
    public/sw_i2c.h
)

mark3_add_library(sw_i2c ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(sw_i2c
    PUBLIC
        public        
    )
    
target_link_libraries(sw_i2c
    mark3
)