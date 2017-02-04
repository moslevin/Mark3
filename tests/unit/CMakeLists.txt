project (ut_base)

set(UT_SOURCES
    unit_test.cpp
    ut_platform.cpp
)
    
set(UT_HEADERS
    unit_test.h
    ut_platform.h
)

mark3_add_library(ut_base ${UT_SOURCES} ${UT_HEADERS})
target_include_directories(ut_base
    PUBLIC
        .
)

target_link_libraries(ut_base
    mark3
    memutil
    uart
)

add_subdirectory(ut_arena)
add_subdirectory(ut_eventflag)
add_subdirectory(ut_heap)
add_subdirectory(ut_logic)
add_subdirectory(ut_mailbox)
add_subdirectory(ut_message)
add_subdirectory(ut_mutex)
add_subdirectory(ut_notify)
add_subdirectory(ut_sanity)
add_subdirectory(ut_semaphore)
add_subdirectory(ut_thread)
add_subdirectory(ut_timer_precision)
add_subdirectory(ut_timers)
