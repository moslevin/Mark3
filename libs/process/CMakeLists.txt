project(process)

set(LIB_SOURCES
    process.cpp
)

set(LIB_HEADERS
    public/process.h
)

mark3_add_library(process ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(process
    PUBLIC
        public        
    )
    
target_link_libraries(process
    heap
    mark3
)