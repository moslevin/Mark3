project(c_lab11_mailboxes)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab11_mailboxes ${BIN_SOURCES})

target_link_libraries(c_lab11_mailboxes.elf
    mark3
    mark3c
)