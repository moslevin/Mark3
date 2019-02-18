project(lab6_timers)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab6_timers ${BIN_SOURCES})

target_link_libraries(lab6_timers.elf
    mark3
)