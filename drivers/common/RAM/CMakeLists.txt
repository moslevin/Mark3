project(drvRAM)

set(LIB_SOURCES
    drvRAM.cpp
)

set(LIB_HEADERS
    public/drvRAM.h
)

mark3_add_library(drvRAM ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(drvRAM
    PUBLIC
        public        
    )
    
target_link_libraries(drvRAM
    driver
    mark3
)
