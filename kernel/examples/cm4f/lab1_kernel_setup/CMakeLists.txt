project(lab1_kernel_setup)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab1_kernel_setup ${BIN_SOURCES})

target_link_libraries(lab1_kernel_setup.elf
    mark3
    stm32f429i-discovery-bsp
    stm32f429-startup
    stm32f4xx-hal-driver
)