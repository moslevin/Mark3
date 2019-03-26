project (ut_notify)

set(UT_SOURCES
    ut_notify.cpp
)
 
mark3_add_executable(ut_notify ${UT_SOURCES})

target_link_libraries(ut_notify.elf
    ut_base
)