project(kernel_profile)

set(UT_SOURCES
    mark3test.cpp
)
 
mark3_add_executable(kernel_profile ${UT_SOURCES})

target_link_libraries(kernel_profile.elf
    mark3
    driver
    ut_support
)
