project(memutil)

set(LIB_SOURCES
    memutil.cpp
)

set(LIB_HEADERS
    public/memutil.h
)

mark3_add_library(memutil ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(memutil 
    PUBLIC
        public        
    )
    
target_link_libraries(memutil
    mark3
)