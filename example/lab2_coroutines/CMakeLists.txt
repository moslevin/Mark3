project(lab2_coroutines)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab2_coroutines ${BIN_SOURCES})

target_link_libraries(lab2_coroutines.elf
    mark3
)
