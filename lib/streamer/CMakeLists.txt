project(streamer)

set(LIB_SOURCES
    streamer.cpp
)

set(LIB_HEADERS
    public/streamer.h
)

mark3_add_library(streamer ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(streamer
    PUBLIC
        public        
    )
    
target_link_libraries(streamer
    mark3
)