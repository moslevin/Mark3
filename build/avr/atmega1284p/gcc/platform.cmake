find_program(AVR_CC avr-gcc PATHS "c:\\Program Files (x86)\\Atmel\\Studio\\7.0\\toolchain\\avr8\\avr8-gnu-toolchain\\bin")
find_program(AVR_CXX avr-g++ PATHS "c:\\Program Files (x86)\\Atmel\\Studio\\7.0\\toolchain\\avr8\\avr8-gnu-toolchain\\bin")
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJDUMP avr-objdump)

set(AVR_CC_FLAGS "\
    -mmcu=atmega328p \
    -funsigned-char \
    -funsigned-bitfields \
    -Os \
    -g3 \
    -fpack-struct \
    -fshort-enums \
    -Wall \
    -c \
    -I~/atmel/avr/include \
    -DAVR \
    ")

set(AVR_CXX_FLAGS "\
    -funsigned-bitfields \
    -Os \
    -g3 \
    -fpack-struct \
    -ffunction-sections \
    -fshort-enums  \
    -Wall \
    -c \
    -mmcu=atmega1284p \
    -I~/atmel/avr/include \
    -DAVR \
    ")

set_property(GLOBAL PROPERTY global_cc "${AVR_CC}")
set_property(GLOBAL PROPERTY global_cxx "${AVR_CXX}")
set_property(GLOBAL PROPERTY global_cc_flags "${AVR_CC_FLAGS}")
set_property(GLOBAL PROPERTY global_cxx_flags "${AVR_CXX_FLAGS}")
