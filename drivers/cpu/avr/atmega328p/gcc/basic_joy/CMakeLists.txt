project(basic_joy)

set(LIB_SOURCES
    drvBasicJoy.cpp
)

set(LIB_HEADERS
    public/drvBasicJoy.h
)

mark3_add_library(basic_joy ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(basic_joy
    PUBLIC
        public        
    )
    
target_link_libraries(basic_joy
    joystick
    mark3
)