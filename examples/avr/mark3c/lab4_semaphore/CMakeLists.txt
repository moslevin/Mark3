project(c_lab4_semaphore)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab4_semaphore ${BIN_SOURCES})

set_target_properties(
    c_lab4_semaphore.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab4_semaphore.elf
    mark3
    mark3c
)
