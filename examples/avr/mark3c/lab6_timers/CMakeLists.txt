project(c_lab6_timers)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab6_timers ${BIN_SOURCES})

target_link_libraries(c_lab6_timers.elf
    mark3
    mark3c
)