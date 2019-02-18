### Set default toolchain configuration
set(mark3_arch      "avr"             CACHE STRING "Target Archictecture (i.e. avr, cm0, cm3, cm4f, msp430)")
set(mark3_variant   "atmega328p"      CACHE STRING "Target Archictecture (i.e. avr, cm0, cm3, cm4f, msp430)")
set(mark3_toolchain "gcc"             CACHE STRING "Target Toolchain name (i.e. gcc)")
set(mark3_root_dir  "$ENV{ROOT_DIR}"  CACHE STRING "Target Toolchain name (i.e. gcc)")

### Emit a notice on the command line indicating the environment configuration
message(STATUS "-----------------------------------------------")
message(STATUS "==[MARK3 CMake Build System]===================")
message(STATUS "-----------------------------------------------")
message(STATUS " CPU Architecture  = ${mark3_arch}")
message(STATUS " CPU Variant       = ${mark3_variant}")
message(STATUS " Toolchain         = ${mark3_toolchain}")
message(STATUS "-----------------------------------------------")

### Include an architecture/variant/toolchain specific platorm configuration.  If the files exist in the
### BSP, prefer that over the files included as part of the kernel's "build" directory. 
if(EXISTS "${mark3_root_dir}/bsp/arch/${mark3_arch}/${mark3_variant}/${mark3_toolchain}/platform.cmake")
include("${mark3_root_dir}/bsp/arch/${mark3_arch}/${mark3_variant}/${mark3_toolchain}/platform.cmake")
set(mark3_has_bsp "true")
else()
include("${mark3_root_dir}/build/arch/${mark3_arch}/${mark3_variant}/${mark3_toolchain}/platform.cmake")
set(mark3_has_bsp "false")
endif()

### Load variables from the platform configuration file into global variables
get_property(mark3_cc GLOBAL PROPERTY global_cc)
get_property(mark3_cxx GLOBAL PROPERTY global_cxx)
get_property(mark3_objcopy GLOBAL PROPERTY global_objcopy)
get_property(mark3_objdump GLOBAL PROPERTY global_objdump)
get_property(mark3_size GLOBAL PROPERTY global_size)
get_property(mark3_cc_flags GLOBAL PROPERTY global_cc_flags)
get_property(mark3_cxx_flags GLOBAL PROPERTY global_cxx_flags)
get_property(mark3_ln_flags GLOBAL PROPERTY global_ln_flags)
get_property(mark3_ln_dbg_flags GLOBAL PROPERTY global_ln_dbg_flags)
get_property(mark3_objcopy_flags GLOBAL PROPERTY global_objcopy_flags)
get_property(mark3_objcopy_dbg_flags GLOBAL PROPERTY global_objcopy_dbg_flags)
get_property(mark3_base_libs GLOBAL PROPERTY global_base_libs)
get_property(mark3_asm_compiler GLOBAL PROPERTY global_asm_compiler)

## Default for embedded systems without a full OS
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

## Prevent system from trying to compile host-executable binaries using toolchain
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

## Suggested cross-compiler boilerplate options
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

## Compile flags
set(CMAKE_SYSTEM_PROCESSOR ${mark3_arch})
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

### Set compiler + toolchain flags
set(CMAKE_C_COMPILER ${mark3_cc})
set(CMAKE_CXX_COMPILER ${mark3_cxx})
set(CMAKE_ASM_COMPILER ${mark3_asm_compiler})
set(CMAKE_CC_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <DEFINES> <FLAGS> -o <OBJECT>   -c <SOURCE>")


