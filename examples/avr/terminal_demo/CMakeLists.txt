project(terminal_demo)

set(BIN_SOURCES
    main.cpp
    nlfs_eeprom.cpp
)
 
mark3_add_executable(terminal_demo ${BIN_SOURCES})

target_link_libraries(terminal_demo.elf
    uartplus 
    mark3 
    terminal 
    memutil 
    nlfs 
    streamer
)