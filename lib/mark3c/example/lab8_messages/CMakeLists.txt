project(c_lab8_messages)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab8_messages ${BIN_SOURCES})

set_target_properties(
    c_lab8_messages.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags}"
        LINK_FLAGS "${mark3_ln_flags}"
    )

target_link_libraries(c_lab8_messages.elf
    mark3
    mark3c
)
