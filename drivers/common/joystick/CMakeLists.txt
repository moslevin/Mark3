project(joystick)

set(LIB_SOURCES
    joystick.cpp
)

set(LIB_HEADERS
    public/joystick.h
)

mark3_add_library(joystick ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(joystick
    PUBLIC
        public        
    )
    
target_link_libraries(joystick
    driver
    mark3
)
