project(terminal)

set(LIB_SOURCES
    command_dispatch.cpp
    command_handler.cpp
    linebuffer.cpp
    m3shell.cpp
    terminal_in.cpp
    terminal_io.cpp
    terminal_out.cpp
)

set(LIB_HEADERS
    public/command_dispatch.h
    public/command_handler.h
    public/linebuffer.h
    public/m3shell.h
    public/terminal_in.h
    public/terminal_io.h
    public/terminal_out.h
)

mark3_add_library(terminal ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(terminal
    PUBLIC
        public        
    )
    
target_link_libraries(terminal
    driver
    mark3
    memutil 
)
