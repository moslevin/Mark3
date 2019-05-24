project (ut_coroutine)

set(UT_SOURCES
    ut_coroutine.cpp
)

mark3_add_executable(ut_coroutine ${UT_SOURCES})

target_link_libraries(ut_coroutine.elf
    heap
    ut_base
)
