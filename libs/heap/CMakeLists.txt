project(heap)

set(LIB_SOURCES
    arena.cpp
    bitmap_allocator.cpp
    fixed_heap.cpp
    heapblock.cpp
    slab.cpp
)

set(LIB_HEADERS
    public/arena.h
    public/arenalist.h
    public/bitmap_allocator.h
    public/fixed_heap.h
    public/heapblock.h
    public/slab.h
)

mark3_add_library(heap ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(heap 
    PUBLIC
        public        
    )
    
target_link_libraries(heap
    mark3   
)
