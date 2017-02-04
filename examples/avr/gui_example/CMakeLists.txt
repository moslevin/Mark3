project(gui_example)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(gui_example ${BIN_SOURCES})

target_link_libraries(gui_example.elf
   mark3 
   heap 
   random 
   graphics 
   arial_10 
   graphics_flavr
   joystick 
   joystick_flavr
   gui 
   memutil
)