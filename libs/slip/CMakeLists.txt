project(slip)

set(LIB_SOURCES
    slip.cpp
)

set(LIB_HEADERS
    public/slip.h
)

mark3_add_library(slip ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(slip
    PUBLIC
        public        
    )
    
target_link_libraries(slip
    mark3
    uart
)
