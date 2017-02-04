project(slipterm)

set(LIB_SOURCES
    slipterm.cpp
)

set(LIB_HEADERS
    public/slipterm.h
)

mark3_add_library(slipterm ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(slipterm
    PUBLIC
        public        
    )
    
target_link_libraries(slipterm
    mark3
    slip
    uart
)