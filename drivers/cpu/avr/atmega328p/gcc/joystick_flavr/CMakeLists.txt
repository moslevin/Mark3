project(joystick_flavr)

set(LIB_SOURCES
    drvFlavrJoy.cpp
)

set(LIB_HEADERS
    public/drvFlavrJoy.h
)

mark3_add_library(joystick_flavr ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(joystick_flavr
    PUBLIC
        public        
    )
    
target_link_libraries(joystick_flavr
    driver
    joystick
    mark3
)
