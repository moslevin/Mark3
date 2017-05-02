project(swuart)

set(LIB_SOURCES
    sw_uart.cpp
)

set(LIB_HEADERS
    public/sw_uart.h
)

mark3_add_library(sw_uart ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(sw_uart
    PUBLIC
        public        
    )
    
target_link_libraries(sw_uart
    mark3
)