project(lab8_messages)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab8_messages ${BIN_SOURCES})

target_link_libraries(lab8_messages.elf
    mark3
)