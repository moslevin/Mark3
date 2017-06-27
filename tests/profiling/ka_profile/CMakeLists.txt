project (ka_profile)

set(UT_SOURCES
    mark3test.cpp
)
 
mark3_add_executable(ka_profile ${UT_SOURCES})

target_link_libraries(ka_profile.elf
    mark3
    atmegauart
)
