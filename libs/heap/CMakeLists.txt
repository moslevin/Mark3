project(heap)

set(LIB_SOURCES
    arena.cpp
    fixed_heap.cpp
    heapblock.cpp
    system_heap.cpp
)

set(LIB_HEADERS
    public/arena.h
    public/arenalist.h
    public/fixed_heap.h
    public/heapblock.h
    public/system_heap.h
    public/system_heap_config.h
)

mark3_add_library(heap ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(heap 
    PUBLIC
        public        
    )
    
target_link_libraries(heap
    mark3   
)