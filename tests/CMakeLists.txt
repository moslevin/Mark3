if(${mark3_arch} STREQUAL "avr")
    add_subdirectory(profiling)
    add_subdirectory(unit)
endif(${mark3_arch} STREQUAL "avr")    
