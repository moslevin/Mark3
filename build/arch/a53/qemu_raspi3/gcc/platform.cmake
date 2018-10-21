find_program(A53_CC aarch64-elf-gcc)
find_program(A53_CXX aarch64-elf-g++)
find_program(A53_OBJCOPY aarch64-elf-objcopy)
find_program(A53_SIZE_TOOL aarch64-elf-size)
find_program(A53_OBJDUMP aarch64-elf-objdump)
find_program(A53_LD aarch64-elf-ld)

#----------------------------------------------------------------------------
set(A53_CC_FLAGS "\
    -Os \
    -g3 \
    -Wall \
    -c \
    -fdata-sections \
    -ffreestanding \
    -fno-stack-protector \
    -nostartfiles \
    -nostdlib \
    ")

set(A53_CXX_FLAGS "\
    -Os \
    -g3 \
    -Wall \
    -c \
    -fdata-sections \
    -ffreestanding \
    -fno-exceptions \
    -fno-use-cxa-atexit \
    -fno-stack-protector \
    -fno-rtti \
    -nostartfiles \
    -nostdlib \
    -std=c++14 \
    ")

set(A53_LN_FLAGS "\
    -nostartfiles \
    -nostdlib \
    -Wl,--start-group \
    -Wl,--default-script=${CMAKE_CURRENT_LIST_DIR}/standalone.ld \
    -Wl,--end-group \
    ")

set(A53_LN_DBG_FLAGS "\
    -nostartfiles \
    -nostdlib \
    -Wl,--start-group \
    -Wl,--default-script=${CMAKE_CURRENT_LIST_DIR}/standalone.ld \
    -Wl,--end-group \
    ")

set(A53_OBJCOPY_FLAGS
    -O binary
    )

set(A53_OBJCOPY_DBG_FLAGS
    --only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0
    )

set(A53_BASE_LIBS
    startup semihosting
    )

#----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY global_cc "${A53_CC}")
set_property(GLOBAL PROPERTY global_cxx "${A53_CXX}")
set_property(GLOBAL PROPERTY global_objcopy "${A53_OBJCOPY}")
set_property(GLOBAL PROPERTY global_objdump "${A53_OBJDUMP}")
set_property(GLOBAL PROPERTY global_size "${A53_SIZE_TOOL}")
set_property(GLOBAL PROPERTY global_cc_flags "${A53_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${A53_CXX_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_flags "${A53_LN_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_dbg_flags "${A53_LN_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_flags "${A53_OBJCOPY_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_dbg_flags "${A53_OBJCOPY_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_base_libs "${A53_BASE_LIBS}")
set_property(GLOBAL PROPERTY global_asm_compiler "${A53_CXX}")
set(CMAKE_LINKER "${A53_LD}")
set(CMAKE_CXX_LINK_EXECUTABLE="<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
