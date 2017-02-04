project(nlfs)

set(LIB_SOURCES
    nlfs.cpp
    nlfs_file.cpp
    nlfs_ram.cpp
)

set(LIB_HEADERS
    public/nlfs.h
    public/nlfs_config.h
    public/nlfs_file.h
    public/nlfs_ram.h
)

mark3_add_library(nlfs ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(nlfs
    PUBLIC
        public        
    )
    
target_link_libraries(nlfs
    mark3
    memutil
)