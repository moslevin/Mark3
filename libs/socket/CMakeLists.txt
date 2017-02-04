project(socket)

set(LIB_SOURCES
    socket.cpp
)

set(LIB_HEADERS
    public/socket.h
)

mark3_add_library(socket ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(socket
    PUBLIC
        public        
    )
    
target_link_libraries(socket
    mark3
    streamer
)