project(c_lab5_mutexes)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab5_mutexes ${BIN_SOURCES})

set_target_properties(
    c_lab5_mutexes.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab5_mutexes.elf
    mark3
    mark3c
)
