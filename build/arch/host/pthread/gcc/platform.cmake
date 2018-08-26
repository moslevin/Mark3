find_program(HOST_CC gcc)
find_program(HOST_CXX g++)
find_program(HOST_OBJCOPY objcopy)
find_program(HOST_SIZE_TOOL size)
find_program(HOST_OBJDUMP objdump)

#----------------------------------------------------------------------------
set(HOST_CC_FLAGS "\    
    -funsigned-char \
    -funsigned-bitfields \
    -O0 \
    -g3 \
    -ffunction-sections \
    -fshort-enums  \
    -fpack-struct \
    -Wall \
    -c \
    ")

set(HOST_CXX_FLAGS "\
    -funsigned-char \
    -funsigned-bitfields \
    -Os \
    -g3 \
    -std=c++11 \
    -fpack-struct \
    -ffunction-sections \
    -fshort-enums  \
    -Wall \
    -c \
    ")

set(HOST_LN_FLAGS "\
    -Wl,-start-group \
    -Wl,-lm  \
    -Wl,--end-group \
    -Wl,--gc-sections \
    ")
    
set(HOST_LN_DBG_FLAGS "\
    -Wl,--start-group \
    -Wl,-lm  \
    -Wl,--end-group \
    -Wl,--section-start=.logger=0x1000000 \
    -mmcu=atmega1284p \
    ")
    
set(HOST_OBJCOPY_FLAGS
    -O ihex
    )
    
set(HOST_OBJCOPY_DBG_FLAGS
    --only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0
    )

set(HOST_BASE_LIBS
    )

#----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY global_cc "${HOST_CC}")
set_property(GLOBAL PROPERTY global_cxx "${HOST_CXX}")
set_property(GLOBAL PROPERTY global_objcopy "${HOST_OBJCOPY}")
set_property(GLOBAL PROPERTY global_objdump "${HOST_OBJDUMP}")
set_property(GLOBAL PROPERTY global_size "${HOST_SIZE_TOOL}")
set_property(GLOBAL PROPERTY global_cc_flags "${HOST_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${HOST_CXX_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_flags "${HOST_LN_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_dbg_flags "${HOST_LN_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_flags "${HOST_OBJCOPY_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_dbg_flags "${HOST_OBJCOPY_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_base_libs "${HOST_BASE_LIBS}")
