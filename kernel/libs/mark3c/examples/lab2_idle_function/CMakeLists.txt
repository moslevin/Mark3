project(c_lab2_idle_function)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab2_idle_function ${BIN_SOURCES})

set_target_properties(
    c_lab2_idle_function.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab2_idle_function.elf
    mark3
    mark3c
)
