project(dcpu)

set(LIB_SOURCES
    dcpu.cpp
    )

set(LIB_HEADERS
    public/dcpu.h
    )

mark3_add_library(dcpu ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(dcpu 
    PUBLIC
        public        
    )
    
target_link_libraries(dcpu
    mark3
)
    