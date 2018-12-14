find_program(CM4_CC arm-none-eabi-gcc)
find_program(CM4_CXX arm-none-eabi-g++)
find_program(CM4_OBJCOPY arm-none-eabi-objcopy)
find_program(CM4_SIZE_TOOL arm-none-eabi-size)
find_program(CM4_OBJDUMP arm-none-eabi-objdump)

#----------------------------------------------------------------------------
set(CM4_CC_FLAGS "\
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
    -mcpu=cortex-m4 \
    -DARM \
    -DUSE_STDPERIPH_DRIVER \
    -D__CHECK_DEVICE_DEFINES \
    -DUSE_FULL_ASSERT \
    -fmessage-length=0 \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16 \
    ")

set(CM4_CXX_FLAGS "\
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
    -mcpu=cortex-m4 \
    -DARM \
    -DUSE_STDPERIPH_DRIVER \
    -D__CHECK_DEVICE_DEFINES \
    -DUSE_FULL_ASSERT \
    -fmessage-length=0 \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16 \
    -std=c++11 \
    ")

set(CM4_LN_FLAGS "\
    --specs=nosys.specs \
    --disable-newlib-supplied-syscalls \
    -Wl,--start-group \
    -Wl,-lm \
    -Wl,--end-group \
    -Wl,--gc-sections \
    -mthumb \
    -mcpu=cortex-m4 \
    -mfloat-abi=soft \
    ")
    
set(CM4_LN_DBG_FLAGS "\
    --specs=nosys.specs \
    --disable-newlib-supplied-syscalls \
    -Wl,--start-group \
    -Wl,-lm \
    -Wl,--end-group \
    -mthumb \
    -mcpu=cortex-m4 \
    -mfloat-abi=soft \
    ")
    
set(CM4_OBJCOPY_FLAGS
    -O ihex -R .eeprom -R .fuse -R .lock -R .signature
    )
    
set(CM4_OBJCOPY_DBG_FLAGS
    --only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0
    )

#----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY global_cc "${CM4_CC}")
set_property(GLOBAL PROPERTY global_cxx "${CM4_CXX}")
set_property(GLOBAL PROPERTY global_objcopy "${CM4_OBJCOPY}")
set_property(GLOBAL PROPERTY global_objdump "${CM4_OBJDUMP}")
set_property(GLOBAL PROPERTY global_size "${CM4_SIZE_TOOL}")
set_property(GLOBAL PROPERTY global_cc_flags "${CM4_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${CM4_CXX_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_flags "${CM4_LN_FLAGS}")
set_property(GLOBAL PROPERTY global_ln_dbg_flags "${CM4_LN_DBG_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_flags "${CM4_OBJCOPY_FLAGS}")
set_property(GLOBAL PROPERTY global_objcopy_dbg_flags "${CM4_OBJCOPY_DBG_FLAGS}")
