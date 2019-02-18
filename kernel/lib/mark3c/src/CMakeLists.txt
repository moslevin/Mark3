project(mark3c)

##-- Normal lib.
set(LIB_SOURCES
    mark3c.cpp
)

set(LIB_HEADERS
    public/fake_types.h
    public/mark3c.h
)

mark3_add_library(mark3c ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(mark3c 
    PUBLIC
        public        
    )

target_link_libraries(mark3c
    mark3
)

