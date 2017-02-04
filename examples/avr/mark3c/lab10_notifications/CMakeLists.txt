project(c_lab10_notifications)

set(BIN_SOURCES
    main.c
)
 
mark3_add_executable(c_lab10_notifications ${BIN_SOURCES})

target_link_libraries(c_lab10_notifications.elf
    mark3
    mark3c
)