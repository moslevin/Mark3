project (ut_readerwriter)

set(UT_SOURCES
    ut_readerwriter.cpp
)

mark3_add_executable(ut_readerwriter ${UT_SOURCES})

target_link_libraries(ut_readerwriter.elf
    ut_base
)
