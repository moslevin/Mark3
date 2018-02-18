find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJDUMP avr-objdump)

#----------------------------------------------------------------------------
set(AVR_CC_FLAGS "\    
    -funsigned-char \
    -funsigned-bitfields \
    -Os \
    -g3 \
    -fpack-struct \
    -fshort-enums \
    -Wall \
    -c \
    -mmcu=atmega328p \
    -DAVR \
    ")

set(AVR_CXX_FLAGS "\
    -funsigned-char \
    -funsigned-bitfields \
    -Os \
    -std=c++11 \
    -g3 \
    -fpack-struct \
    -ffunction-sections \
    -fshort-enums  \
    -Wall \
    -c \
    -mmcu=atmega328p \
    -DAVR \
    ")

set(AVR_LN_FLAGS "\
    -Wl,-start-group \
    -Wl,-lm  \
    -Wl,--end-group \
    -Wl,--gc-sections \
    -mmcu=atmega328p \
    ")
    
set(AVR_LN_DBG_FLAGS "\
    -Wl,--start-group \
    -Wl,-lm  \
    -Wl,--end-group \
    -Wl,--section-start=.logger=0x1000000 \
    -mmcu=atmega328p \
    ")
    
set(AVR_OBJCOPY_FLAGS
    -O ihex -R .eeprom -R .fuse -R .lock -R .signature
    )
    
set(AVR_OBJCOPY_DBG_FLAGS
    --only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0
    )

#----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY global_cc "${AVR_CC}")
set_property(GLOBAL PROPERTY global_cxx "${AVR_CXX}")
set_property(GLOBAL PROPERTY global_objcopy "${AVR_OBJCOPY}")
set_property(GLOBAL PROPERTY global_objdump "${AVR_OBJDUMP}")
set_property(GLOBAL PROPERTY global_size "${AVR_SIZE_TOOL}")
set_property(GLOBAL PROPERTY global_cc_flags "${AVR_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${AVR_CXX_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_flags "${AVR_LN_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_dbg_flags "${AVR_LN_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_flags "${AVR_OBJCOPY_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_dbg_flags "${AVR_OBJCOPY_DBG_FLAGS}")
