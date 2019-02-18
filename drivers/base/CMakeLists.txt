project(driver)

set(LIB_SOURCES
    driver.cpp
)

set(LIB_HEADERS
    public/driver.h
)

mark3_add_library(driver ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(driver
    PUBLIC
        public        
    )
    
target_link_libraries(driver
    mark3
)
