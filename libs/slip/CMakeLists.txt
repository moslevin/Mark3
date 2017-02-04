project(slip)

set(LIB_SOURCES
    slip.cpp
    slip_mux.cpp
)

set(LIB_HEADERS
    public/slip.h
    public/slip_mux.h
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