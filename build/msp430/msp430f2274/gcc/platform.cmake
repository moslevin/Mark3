find_program(MSP_CC msp430-elf-gcc)
find_program(MSP_CXX msp430-elf-g++)
find_program(MSP_OBJCOPY msp430-elf-objcopy)
find_program(MSP_SIZE_TOOL msp430-elf-size)
find_program(MSP_OBJDUMP msp430-elf-objdump)

#----------------------------------------------------------------------------
set(MSP_CC_FLAGS "\
    -funsigned-char \
    -funsigned-bitfields \
    -O0 \
    -g3 \
    -fpack-struct \
    -fshort-enums \
    -Wall \
    -c \
    -mmcu=msp430f2274 \
    -DMSP430 \
    -I\"C:\\Program Files (x86)\\TI\\MSP430\\include\"
    ")

set(MSP_CXX_FLAGS "\
    -funsigned-char \
    -funsigned-bitfields \
    -fno-exceptions \
    -fno-rtti \
    -O0 \
    -g3 \
    -fpack-struct \
    -fshort-enums \
    -Wall \
    -c \
    -mmcu=msp430f2274 \
    -DMSP430 \
    -IC:\"\\Program Files\ (x86)\\TI\\MSP430\\include\"
    ")

set(MSP_LN_FLAGS "\
    --specs=nosys.specs \
    --disable-newlib-supplied-syscalls \
    -Wl,--start-group \
    -Wl,-lm \
    -Wl,--end-group \
    -Wl,--gc-sections \
    -mmcu=msp430f2274
    ")
    
set(MSP_LN_DBG_FLAGS "\
    --specs=nosys.specs \
    --disable-newlib-supplied-syscalls \
    -Wl,--start-group \
    -Wl,-lm \
    -Wl,--end-group \
    -Wl,--section-start=.logger=0x1000000 \
    -mmcu=msp430f2274
    ")
    
set(MSP_OBJCOPY_FLAGS
    -O ihex
    )
    
set(MSP_OBJCOPY_DBG_FLAGS
    --only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0
    )

#----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY global_cc "${MSP_CC}")
set_property(GLOBAL PROPERTY global_cxx "${MSP_CXX}")
set_property(GLOBAL PROPERTY global_objcopy "${MSP_OBJCOPY}")
set_property(GLOBAL PROPERTY global_objdump "${MSP_OBJDUMP}")
set_property(GLOBAL PROPERTY global_size "${MSP_SIZE_TOOL}")
set_property(GLOBAL PROPERTY global_cc_flags "${MSP_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${MSP_CXX_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_flags "${MSP_LN_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_dbg_flags "${MSP_LN_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_flags "${MSP_OBJCOPY_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_dbg_flags "${MSP_OBJCOPY_DBG_FLAGS}")
