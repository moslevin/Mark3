project(buffalogger)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(buffalogger ${BIN_SOURCES})

target_link_libraries(buffalogger.elf
    mark3
    uart
)