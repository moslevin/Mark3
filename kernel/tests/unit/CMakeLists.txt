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
    atmegauart
)

subdirlist(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
foreach(subdir ${SUBDIRS})
  add_subdirectory(${subdir})
endforeach()
