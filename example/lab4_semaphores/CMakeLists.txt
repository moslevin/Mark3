project(lab4_semaphores)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab4_semaphores ${BIN_SOURCES})

target_link_libraries(lab4_semaphores.elf
    mark3
)