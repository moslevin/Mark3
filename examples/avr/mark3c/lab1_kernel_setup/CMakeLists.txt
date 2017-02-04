project(c_lab1_kernel_setup)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab1_kernel_setup ${BIN_SOURCES})

target_link_libraries(c_lab1_kernel_setup.elf
    mark3
    mark3c
)