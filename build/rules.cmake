#----------------------------------------------------------------------------
function(mark3_add_executable_i TARGET_NAME C_FLAGS)
    set(raw_image   ${TARGET_NAME}.img)
    set(elf_image   ${TARGET_NAME}.elf)
    set(bin_image   ${TARGET_NAME}.bin)
    set(dbg_image   ${TARGET_NAME}.dbg)
    set(map_file    ${TARGET_NAME}.map)

    # Rule for building the main executable
    add_executable(${elf_image} ${ARGN})
    set_target_properties(${elf_image}
        PROPERTIES
            COMPILE_FLAGS   "${C_FLAGS}"
            LINK_FLAGS      "${mark3_ln_flags} -Wl,-Map=${map_file}"
    )

    target_link_libraries(${elf_image} ${mark3_base_libs})

    # Rule for building the hex image
    add_custom_command(
        OUTPUT ${raw_image}
        COMMAND
            ${mark3_objcopy} ${mark3_objcopy_flags} ${elf_image} ${raw_image}
        DEPENDS
            ${elf_image}
    )

    # Rule for building the bin image
    add_custom_command(
        OUTPUT ${bin_image}
        COMMAND
            ${mark3_objcopy} -O binary ${elf_image} ${bin_image}
        DEPENDS
            ${elf_image}
    )

    # Rule for building the Buffalogger symbols image
    if(EXISTS "/dev/null")
        set(null_file /dev/null)
    else(EXISTS "/dev/null")
        set(null_file nul)
    endif(EXISTS "/dev/null")

    add_custom_command(
        OUTPUT ${dbg_image}
        COMMAND
            ${mark3_objcopy} ${mark3_objcopy_dbg_flags} ${elf_image} ${dbg_image} > ${null_file} 2>&1
        DEPENDS
            ${elf_image}
    )

    # Ensure that the hex, bin, and debug images are always built for the target
    add_custom_target(${TARGET_NAME}
        ALL
        DEPENDS ${bin_image} ${raw_image} ${dbg_image}
    )

    # Map the target name to the elf image
    set_target_properties(${TARGET_NAME}
        PROPERTIES
            OUTPUT_NAME ${elf_image}
    )

    get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
    set_directory_properties(
      PROPERTIES
         ADDITIONAL_MAKE_CLEAN_FILES "${map_file}"
    )
endfunction(mark3_add_executable_i)

#----------------------------------------------------------------------------
function(mark3_add_executable TARGET_NAME)
    mark3_add_executable_i(${TARGET_NAME} ${mark3_cxx_flags} ${ARGN})
endfunction(mark3_add_executable)

#----------------------------------------------------------------------------
function(mark3_add_executable_c TARGET_NAME)
    mark3_add_executable_i(${TARGET_NAME} ${mark3_cc_flags} ${ARGN})
endfunction(mark3_add_executable_c)

#----------------------------------------------------------------------------
function(mark3_add_library_c TARGET_NAME)
    add_library(${TARGET_NAME} ${ARGN})
    set_target_properties(
        ${TARGET_NAME}
        PROPERTIES
            COMPILE_FLAGS   "${mark3_cc_flags}"
    )

endfunction(mark3_add_library_c)

#----------------------------------------------------------------------------
function(mark3_add_library TARGET_NAME)
    add_library(${TARGET_NAME} ${ARGN})
    set_target_properties(
        ${TARGET_NAME}
        PROPERTIES
            COMPILE_FLAGS   "${mark3_cxx_flags}"
    )

endfunction(mark3_add_library)

#----------------------------------------------------------------------------
macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()
