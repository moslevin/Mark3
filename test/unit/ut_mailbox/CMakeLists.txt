project (ut_mailbox)

set(UT_SOURCES
    ut_mailbox.cpp
)
 
mark3_add_executable(ut_mailbox ${UT_SOURCES})

target_link_libraries(ut_mailbox.elf
    ut_base
)