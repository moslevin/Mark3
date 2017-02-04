project(c_lab9_dynamic_threads)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab9_dynamic_threads ${BIN_SOURCES})

target_link_libraries(c_lab9_dynamic_threads.elf
    mark3
    mark3c
)