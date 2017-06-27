project(font_test)

set(BIN_SOURCES
    Kernel++.cpp
)
 
mark3_add_executable(font_test ${BIN_SOURCES})

target_link_libraries(font_test.elf
    mark3 
    slip 
    uart
    arial_10
    atmegauart
)
