project(uart)

set(LIB_SOURCES
    drvUART.cpp
)

set(LIB_HEADERS
    public/drvUART.h
)

mark3_add_library(uart ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(uart
    PUBLIC
        public        
    )
    
target_link_libraries(uart
    mark3
)