project(c_lab9_dynamic_threads)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab9_dynamic_threads ${BIN_SOURCES})

set_target_properties(
    c_lab9_dynamic_threads.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab9_dynamic_threads.elf
    mark3
    mark3c
)
