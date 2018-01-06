project(uartplus)

set(LIB_SOURCES
    drvUARTplus.cpp
)

set(LIB_HEADERS
    public/drvUARTplus.h
)

mark3_add_library(uartplus ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(uartplus
    PUBLIC
        public
    )

target_link_libraries(uartplus
    driver
    mark3
    streamer
    uart
)
