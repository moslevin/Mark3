project(psx)

set(LIB_SOURCES
    drvPSXJoy.cpp
)

set(LIB_HEADERS
    public/drvPSXJoy.h
)

mark3_add_library(psx ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(psx
    PUBLIC
        public        
    )
    
target_link_libraries(psx
    joystick
    mark3
)