project(c_lab6_timers)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab6_timers ${BIN_SOURCES})

set_target_properties(
    c_lab6_timers.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab6_timers.elf
    mark3
    mark3c
)
