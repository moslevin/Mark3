project(brickbreak)

set(BIN_SOURCES
    brball.cpp
    brcommon.cpp
    breakout.cpp
    brlevel.cpp
    brlives.cpp 
    brpaddle.cpp
    brplayfield.cpp
    brscore.cpp
    font_8.cpp
    highscore.cpp
    markade2.cpp
)
 
mark3_add_executable(brickbreak ${BIN_SOURCES})

target_link_libraries(brickbreak.elf
    mark3 
    sound 
    joystick_flavr
    basic_joy
    joystick 
    graphics_flavr 
    graphics_st7735 
    graphics 
    memutil
)