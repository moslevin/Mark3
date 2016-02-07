# Platform-specific options

USES_CXX=1

CC=arm-none-eabi-gcc
CPP=arm-none-eabi-g++

### Generic compiler flags. ###
CXFLAGS=-Os                 \
        -g3                 \
        -Wall               \
        -c                  \
        -fdata-sections     \
        -ffunction-sections \
        -ffreestanding      \
        -DK_ADDR=uint32_t   \
        -DK_WORD=uint32_t

### These values are generic and may be overridden when building for a particular target ###.
ARM_C_FLAGS=-mthumb                     \
            -mcpu=cortex-m3             \
            -DARM                       \
            -DUSE_STDPERIPH_DRIVER      \
            -D__CHECK_DEVICE_DEFINES    \
            -DUSE_FULL_ASSERT           \
            -fmessage-length=0          \
            -mfloat-abi=hard            \
            -mfpu=fpv4-sp-d16

CFLAGS=$(CXFLAGS) $(ARM_C_FLAGS)
CPPFLAGS=$(CXFLAGS) $(ARM_C_FLAGS) -fno-rtti

LINK=arm-none-eabi-gcc
LFLAGS=-Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nano.specs --specs=nosys.specs
LFLAGS_DBG=-Wl,--start-group -Wl,-lm  -Wl,--end-group -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nano.specs --specs=nosys.specs

AR=arm-none-eabi-ar
ARFLAGS=rcs

ASM=arm-none-eabi-as
ASMFLAGS=-mcpu=cortex-m4

OBJCOPY=arm-none-eabi-objcopy
OBJCOPY_FLAGS=-O ihex -R .eeprom -R .fuse -R .lock -R .signature
OBJCOPY_DBG_FLAGS=--only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0

