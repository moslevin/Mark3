project(c_lab5_mutexes)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab5_mutexes ${BIN_SOURCES})

target_link_libraries(c_lab5_mutexes.elf
    mark3
    mark3c
)