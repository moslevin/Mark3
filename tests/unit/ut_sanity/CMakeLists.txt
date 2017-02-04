project (ut_sanity)

set(UT_SOURCES
    ut_sanity.cpp
)
 
mark3_add_executable(ut_sanity ${UT_SOURCES})

target_link_libraries(ut_sanity.elf
    ut_base
)