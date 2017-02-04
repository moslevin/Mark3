project(c_lab2_idle_function)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab2_idle_function ${BIN_SOURCES})

target_link_libraries(c_lab2_idle_function.elf
    mark3
    mark3c
)