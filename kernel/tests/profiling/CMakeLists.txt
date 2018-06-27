if("${mark3_arch}" STREQUAL "avr")
    add_subdirectory(ka_profile)
    add_subdirectory(kernel_profiling)
endif()

