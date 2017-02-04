project(c_lab8_messages)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab8_messages ${BIN_SOURCES})

target_link_libraries(c_lab8_messages.elf
    mark3
    mark3c
)