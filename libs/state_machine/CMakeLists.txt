project(state_machine)

set(LIB_SOURCES
    state_machine.cpp
)

set(LIB_HEADERS
    public/state_machine.h
)

mark3_add_library(state_machine ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(state_machine
    PUBLIC
        public        
    )
    
target_link_libraries(state_machine
)