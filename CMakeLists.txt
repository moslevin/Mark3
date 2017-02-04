cmake_minimum_required (VERSION 3.4)
project (Mark3)

# This contains all of the build rules for custom binaries.
include("./build/rules.cmake")

add_subdirectory(bootloader) 
add_subdirectory(kernel)
add_subdirectory(drivers)
add_subdirectory(libs)
add_subdirectory(tests)
add_subdirectory(apps)
add_subdirectory(examples)
add_subdirectory(fonts)