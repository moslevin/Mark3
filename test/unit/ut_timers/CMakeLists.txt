project (ut_timers)

set(UT_SOURCES
    ut_timers.cpp
)
 
mark3_add_executable(ut_timers ${UT_SOURCES})

target_link_libraries(ut_timers.elf
    ut_base
)