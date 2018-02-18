project(lab3_round_robin)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab3_round_robin ${BIN_SOURCES})

target_link_libraries(lab3_round_robin.elf
    mark3
)