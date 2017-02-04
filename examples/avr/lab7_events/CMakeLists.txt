project(lab7_events)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab7_events ${BIN_SOURCES})

target_link_libraries(lab7_events.elf
    mark3
)