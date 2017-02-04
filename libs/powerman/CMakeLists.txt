project(powerman)

set(LIB_SOURCES
    powerman.cpp
)

set(LIB_HEADERS
    public/powerman.h
)

mark3_add_library(powerman ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(powerman
    PUBLIC
        public        
    )
    
target_link_libraries(powerman    
    mark3
    memutil
)