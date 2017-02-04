project(lab10_notifications)

set(BIN_SOURCES
    main.cpp
)
 
mark3_add_executable(lab10_notifications ${BIN_SOURCES})

target_link_libraries(lab10_notifications.elf
    mark3
)