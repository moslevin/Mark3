project (ut_logic)

set(UT_SOURCES
    ut_logic.cpp
)

mark3_add_executable(ut_logic ${UT_SOURCES})

target_link_libraries(ut_logic.elf
    heap
    ut_base
)
