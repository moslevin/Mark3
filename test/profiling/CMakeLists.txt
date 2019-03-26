if("${mark3_arch}" STREQUAL "avr")
    add_subdirectory(ka_profile)
endif()
add_subdirectory(kernel_profiling)

