project(lab5_mutexes)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab5_mutexes ${BIN_SOURCES})

target_link_libraries(lab5_mutexes.elf
    mark3
)