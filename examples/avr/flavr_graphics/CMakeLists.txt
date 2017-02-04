project(flavr_graphics)

set(BIN_SOURCES
    flavr_gfx.cpp
)
 
mark3_add_executable(flavr_graphics ${BIN_SOURCES})

target_link_libraries(flavr_graphics.elf
    mark3 
    graphics_flavr 
    graphics 
    tahoma_10
    uart
)