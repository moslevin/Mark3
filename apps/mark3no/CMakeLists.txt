project(mark3no)

set(BIN_SOURCES
    main.cpp
    led.cpp
    song.cpp
    button.cpp
    bsp_buttons.cpp
    bsp_heap.cpp
    bsp_leds.cpp
    bsp_rtc.cpp
    bsp_uarts.cpp
    bsp_sound.cpp
)
 
mark3_add_executable(mark3no ${BIN_SOURCES})

target_link_libraries(mark3no.elf
    mark3 
    sound
    uartplus    
    streamer 
    rtc 
    memutil 
    heap 
    terminal
)