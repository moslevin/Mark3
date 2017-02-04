project(c_lab4_semaphore)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab4_semaphore ${BIN_SOURCES})

target_link_libraries(c_lab4_semaphore.elf
    mark3
    mark3c
)