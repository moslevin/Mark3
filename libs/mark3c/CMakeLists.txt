project(mark3c)

##-- Driver interface - special case, c-only code
add_library(mark3c_c driver3c.c public/driver3c.h)
set_target_properties(
    mark3c_c
    PROPERTIES
        COMPILE_FLAGS   "${mark3_cc_flags}"
)

target_include_directories(mark3c_c
    PUBLIC
        public
    )

target_link_libraries(mark3c_c
    mark3
)

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
    mark3c_c
)

