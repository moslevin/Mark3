project(sound)

set(LIB_SOURCES
    drvSound.cpp
)

set(LIB_HEADERS
    public/drvSound.h
)

mark3_add_library(sound ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(sound
    PUBLIC
        public        
    )
    
target_link_libraries(sound
    driver
    mark3
)
