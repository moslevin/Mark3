project(lab11_mailboxes)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab11_mailboxes ${BIN_SOURCES})

target_link_libraries(lab11_mailboxes.elf
    mark3
)