cmake_minimum_required (VERSION 3.4)
project (Mark3)

# This contains all of the build rules for custom binaries.
include("./build/rules.cmake")

if("${mark3_arch}" STREQUAL "avr")
    add_subdirectory(tests)
endif("${mark3_arch}" STREQUAL "avr")    

add_subdirectory(examples)
add_subdirectory(kernel)
add_subdirectory(drivers)
add_subdirectory(libs)
add_subdirectory(bsp)
