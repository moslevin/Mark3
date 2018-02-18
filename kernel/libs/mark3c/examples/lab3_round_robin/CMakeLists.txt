project(c_lab3_round_robin)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab3_round_robin ${BIN_SOURCES})

set_target_properties(
    c_lab3_round_robin.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab3_round_robin.elf
    mark3
    mark3c
)
