project(c_lab3_round_robin)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab3_round_robin ${BIN_SOURCES})

target_link_libraries(c_lab3_round_robin.elf
    mark3
    mark3c
)