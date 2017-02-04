project(mark3c)

set(LIB_SOURCES
    driver3c.c
    mark3c.cpp
)

set(LIB_HEADERS
    public/driver3c.h
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