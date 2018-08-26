project(lab9_dynamic_threads)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab9_dynamic_threads ${BIN_SOURCES})

target_link_libraries(lab9_dynamic_threads.elf
    mark3
    memutil
)