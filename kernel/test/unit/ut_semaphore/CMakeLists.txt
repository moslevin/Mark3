project (ut_semaphore)

set(UT_SOURCES
    ut_semaphore.cpp
)
 
mark3_add_executable(ut_semaphore ${UT_SOURCES})

target_link_libraries(ut_semaphore.elf
    ut_base
)