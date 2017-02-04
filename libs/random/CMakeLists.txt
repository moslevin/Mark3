project(random)

set(LIB_SOURCES
    rand_lfsr.cpp
)

set(LIB_HEADERS
    public/rand_lfsr.h
)

mark3_add_library(random ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(random
    PUBLIC
        public        
    )
    
target_link_libraries(random
    mark3
)