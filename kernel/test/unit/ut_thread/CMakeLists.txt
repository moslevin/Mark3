project (ut_thread)

set(UT_SOURCES
    ut_thread.cpp
)
 
mark3_add_executable(ut_thread ${UT_SOURCES})

target_link_libraries(ut_thread.elf
    ut_base
)