project (ut_message)

set(UT_SOURCES
    ut_message.cpp
)
 
mark3_add_executable(ut_message ${UT_SOURCES})

target_link_libraries(ut_message.elf
    ut_base
)