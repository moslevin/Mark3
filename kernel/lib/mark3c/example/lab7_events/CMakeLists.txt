project(c_lab7_events)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab7_events ${BIN_SOURCES})

set_target_properties(
    c_lab7_events.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab7_events.elf
    mark3
    mark3c
)
