option(BUILD_DOC "Build documentation" OFF)

if (BUILD_DOC)
    add_custom_target(size_profile ALL
        COMMAND bash ./build/tools/size_profile.sh ${mark3_arch} ${mark3_variant} ${mark3_toolchain} -d > ./kernel/src/public/sizeprofile.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../
        COMMENT "Size Profiling"
        VERBATIM )
    add_dependencies(size_profile mark3)

    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Docs"
            VERBATIM )

        add_dependencies( doc_doxygen size_profile )

    else (DOXYGEN_FOUND)
      message("Skipping Doxygen documentation -- Doxygen not found")
    endif (DOXYGEN_FOUND)
endif (BUILD_DOC)
