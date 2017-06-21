project(sw_i2c_demo)

set(BIN_SOURCES
    main.cpp   
)
 
mark3_add_executable(sw_i2c_demo ${BIN_SOURCES})

target_link_libraries(sw_i2c_demo.elf
    uartplus 
    mark3     
    sw_i2c
    memutil
)
