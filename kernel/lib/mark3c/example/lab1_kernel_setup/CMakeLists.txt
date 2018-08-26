project(c_lab1_kernel_setup)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab1_kernel_setup ${BIN_SOURCES})

set_target_properties(
    c_lab1_kernel_setup.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab1_kernel_setup.elf
    mark3
    mark3c
)
