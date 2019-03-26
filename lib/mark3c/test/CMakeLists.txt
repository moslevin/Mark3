project(ut_mark3c)

set(UT_SOURCES
    ut_mark3c.cpp
)

mark3_add_executable(ut_mark3c ${UT_SOURCES})

target_link_libraries(ut_mark3c.elf
    ut_base
    mark3
    mark3c
    memutil
)
