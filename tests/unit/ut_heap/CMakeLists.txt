project (ut_heap)

set(UT_SOURCES
    ut_heap.cpp
)
 
mark3_add_executable(ut_heap ${UT_SOURCES})

target_link_libraries(ut_heap.elf
    heap
    ut_base
)