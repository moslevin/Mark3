project(rtc)

set(LIB_SOURCES
    rtc.cpp
)

set(LIB_HEADERS
    public/rtc.h
)

mark3_add_library(rtc ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(rtc
    PUBLIC
        public        
    )
    
target_link_libraries(rtc
    mark3
)