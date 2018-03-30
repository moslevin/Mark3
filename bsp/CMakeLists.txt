set(ARCH_PATH "./arch/${mark3_arch}/${mark3_variant}/${mark3_toolchain}") 
add_subdirectory(${ARCH_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${mark3_variant})
