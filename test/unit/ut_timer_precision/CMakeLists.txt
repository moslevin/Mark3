project (ut_timer_precision)

set(UT_SOURCES
    ut_timer_sanity.cpp
)
 
mark3_add_executable(ut_timer_precision ${UT_SOURCES})

target_link_libraries(ut_timer_precision.elf
    ut_base
)