project(nlfs_test)

set(BIN_SOURCES
    nlfs_eeprom.cpp
    nlfs_test.cpp
)
 
mark3_add_executable(nlfs_test ${BIN_SOURCES})

target_link_libraries(nlfs_test.elf
    mark3
    uart
    nlfs
    memutil
    slip
    heap
)