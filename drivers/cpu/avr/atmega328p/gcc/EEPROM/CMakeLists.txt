project(eeprom)

set(LIB_SOURCES
    drvEEPROM.cpp
)

set(LIB_HEADERS
    public/drvEEPROM.h
)

mark3_add_library(eeprom ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(eeprom
    PUBLIC
        public        
    )
    
target_link_libraries(eeprom
    driver
    mark3
)
