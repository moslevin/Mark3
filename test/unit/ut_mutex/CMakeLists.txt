project (ut_mutex)

set(UT_SOURCES
    ut_mutex.cpp
)
 
mark3_add_executable(ut_mutex ${UT_SOURCES})

target_link_libraries(ut_mutex.elf
    ut_base
)