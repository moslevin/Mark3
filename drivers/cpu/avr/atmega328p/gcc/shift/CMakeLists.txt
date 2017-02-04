project(shift)

set(LIB_SOURCES
    shiftreg.cpp
)

set(LIB_HEADERS
    public/shiftreg.h
)

mark3_add_library(shift ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(shift
    PUBLIC
        public        
    )
    
target_link_libraries(shift
    mark3
)