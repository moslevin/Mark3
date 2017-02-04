project(bootloader)

set(BIN_SOURCES
    bootloader.c
    )

mark3_add_executable(bootloader ${BIN_SOURCES})
set_target_properties(
    bootloader.elf
    PROPERTIES
        COMPILE_FLAGS "${mark3_cc_flags} -DPAGE_SIZE=256 -DSYSTEM_FREQ=20000000 -DBAUD_RATE=115200"
        LINK_FLAGS "${mark3_ln_flags} -Wl,-Map=bootloader.map -Wl,-section-start=.text=0x1e000"
    )
