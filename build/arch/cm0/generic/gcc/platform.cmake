find_program(CM0_CC arm-none-eabi-gcc)
find_program(CM0_CXX arm-none-eabi-g++)
find_program(CM0_OBJCOPY arm-none-eabi-objcopy)
find_program(CM0_SIZE_TOOL arm-none-eabi-size)
find_program(CM0_OBJDUMP arm-none-eabi-objdump)

#----------------------------------------------------------------------------
set(CM0_CC_FLAGS "\    
    -Os \
    -g3 \
    -Wall \
    -c \
    -fdata-sections \
    -ffunction-sections \
    -ffreestanding \
    -fno-exceptions \
    -nostartfiles \
    -mthumb \
    -mcpu=cortex-m0 \
    -DARM \
    -DUSE_STDPERIPH_DRIVER \
    -D__CHECK_DEVICE_DEFINES \
    -DUSE_FULL_ASSERT \
    -fmessage-length=0
    ")

set(CM0_CXX_FLAGS "\
    -Os \
    -g3 \
    -Wall \
    -c \
    -fdata-sections \
    -ffunction-sections \
    -ffreestanding \
    -fno-exceptions \
    -fno-rtti \
    -nostartfiles \
    -mthumb \
    -mcpu=cortex-m0 \
    -DARM \
    -DUSE_STDPERIPH_DRIVER \
    -D__CHECK_DEVICE_DEFINES \
    -DUSE_FULL_ASSERT \
    -fmessage-length=0 \
    -std=c++11 \
    ")

set(CM0_LN_FLAGS "\
    --specs=nosys.specs \
    --disable-newlib-supplied-syscalls \
    -Wl,--start-group \
    -Wl,-lm \
    -Wl,--end-group \
    -Wl,--gc-sections \
    -mthumb \
    -mcpu=cortex-m0 \
    ")
    
set(CM0_LN_DBG_FLAGS "\
    --specs=nosys.specs \
    --disable-newlib-supplied-syscalls \
    -Wl,--start-group \
    -Wl,-lm \
    -Wl,--end-group \
    -mthumb \
    -mcpu=cortex-m0 \
    ")
    
set(CM0_OBJCOPY_FLAGS
    -O ihex -R .eeprom -R .fuse -R .lock -R .signature
    )
    
set(CM0_OBJCOPY_DBG_FLAGS
    --only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0
    )

#----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY global_cc "${CM0_CC}")
set_property(GLOBAL PROPERTY global_cxx "${CM0_CXX}")
set_property(GLOBAL PROPERTY global_objcopy "${CM0_OBJCOPY}")
set_property(GLOBAL PROPERTY global_objdump "${CM0_OBJDUMP}")
set_property(GLOBAL PROPERTY global_size "${CM0_SIZE_TOOL}")
set_property(GLOBAL PROPERTY global_cc_flags "${CM0_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${CM0_CXX_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_flags "${CM0_LN_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_dbg_flags "${CM0_LN_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_flags "${CM0_OBJCOPY_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_dbg_flags "${CM0_OBJCOPY_DBG_FLAGS}")
