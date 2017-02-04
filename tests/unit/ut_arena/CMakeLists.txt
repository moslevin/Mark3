project (ut_arena)

set(UT_SOURCES
    ut_arena.cpp
)
 
mark3_add_executable(ut_arena ${UT_SOURCES})

target_link_libraries(ut_arena.elf
    heap
    ut_base
)