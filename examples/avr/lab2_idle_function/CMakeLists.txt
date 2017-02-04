project(lab2_idle_function)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab2_idle_function ${BIN_SOURCES})

target_link_libraries(lab2_idle_function.elf
    mark3
)