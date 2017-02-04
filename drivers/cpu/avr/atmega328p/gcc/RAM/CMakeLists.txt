project(ram)

set(LIB_SOURCES
    drvRAM.cpp
)

set(LIB_HEADERS
    public/drvRAM.h
)

mark3_add_library(ram ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(ram
    PUBLIC
        public        
    )
    
target_link_libraries(ram
    mark3
)