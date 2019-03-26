project (ut_condvar)

set(UT_SOURCES
    ut_condvar.cpp
)

mark3_add_executable(ut_condvar ${UT_SOURCES})

target_link_libraries(ut_condvar.elf
    ut_base
)
