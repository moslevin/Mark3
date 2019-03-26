add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(doc)
if ("${mark3_has_bsp}" STREQUAL "true")
    add_subdirectory(test)
    add_subdirectory(example)
endif()

