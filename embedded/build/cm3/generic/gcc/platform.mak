# Platform-specific options

USES_CXX=1

CC=arm-none-eabi-gcc
CPP=arm-none-eabi-g++

CFLAGS=-O0 -g3 -Wall -c -mthumb -mcpu=cortex-m4 -DARM  -DUSE_STDPERIPH_DRIVER -D__CHECK_DEVICE_DEFINES -DUSE_FULL_ASSERT -fmessage-length=0 -fdata-sections -ffunction-sections -DK_ADDR=uint32_t -DK_WORD=uint32_t  -nostdlib
CPPFLAGS=-O0 -g3 -Wall -c -mthumb -mcpu=cortex-m4 -DARM  -DUSE_STDPERIPH_DRIVER -D__CHECK_DEVICE_DEFINES -DUSE_FULL_ASSERT -fmessage-length=0 -fdata-sections -ffunction-sections -DK_ADDR=uint32_t -DK_WORD=uint32_t -fno-rtti -nostdlib

LINK=arm-none-eabi-g++
LFLAGS=-Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -Wl,--library=nosys -mthumb -mcpu=cortex-m4 -T$(ROOT_DIR)/ext/f4/startup/Linker.ld
LFLAGS_DBG=-Wl,--start-group -Wl,-lm  -Wl,--end-group -mthumb -mcpu=cortex-m4 -T$(ROOT_DIR)/ext/f4/startup/Linker.ld

AR=arm-none-eabi-ar
ARFLAGS=rcs

ASM=arm-none-eabi-as
ASMFLAGS=-mcpu=cortex-m4

OBJCOPY=arm-none-eabi-objcopy
OBJCOPY_FLAGS=-O ihex -R .eeprom -R .fuse -R .lock -R .signature
OBJCOPY_DBG_FLAGS=--only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0

CFLAGS+=-I$(ROOT_DIR)/ext/Drivers/CMSIS/Include/ -I~/gcc-arm-none-eabi-4_9-2015q1/arm-none-eabi/include/  -I$(ROOT_DIR)/ext/f4/cmsis/Device/ST/STM32F4xx/Include/ -I$(ROOT_DIR)/ext/f4/cmsis/Include
CPPFLAGS+=-I$(ROOT_DIR)/ext/f4/cmsis/Include/ -I~/gcc-arm-none-eabi-4_9-2015q1/arm-none-eabi/include/ -I$(ROOT_DIR)/ext/f4/cmsis/Device/ST/STM32F4xx/Include/ -I$(ROOT_DIR)/ext/f4/cmsis/Include

