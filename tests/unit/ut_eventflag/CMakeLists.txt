project (ut_eventflag)

set(UT_SOURCES
    ut_eventflag.cpp
)
 
mark3_add_executable(ut_eventflag ${UT_SOURCES})

target_link_libraries(ut_eventflag.elf
    ut_base
)