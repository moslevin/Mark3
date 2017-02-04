project(c_lab7_events)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab7_events ${BIN_SOURCES})

target_link_libraries(c_lab7_events.elf
    mark3
    mark3c
)