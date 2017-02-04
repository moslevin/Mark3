project(gui)

set(LIB_SOURCES
    control_brushpanel.cpp
    control_button.cpp
    control_checkbox.cpp
    control_gamepanel.cpp
    control_groupbox.cpp
    control_label.cpp
    control_notification.cpp
    control_panel.cpp
    control_progress.cpp
    control_sevenseg.cpp
    control_slickbutton.cpp
    control_slickgroupbox.cpp
    control_slickprogress.cpp
    gui.cpp
)

set(LIB_HEADERS
    public/control_brushpanel.h
    public/control_button.h
    public/control_checkbox.h
    public/control_gamepanel.h
    public/control_groupbox.h
    public/control_label.h
    public/control_notification.h
    public/control_panel.h
    public/control_progress.h
    public/control_sevenseg.h
    public/control_slickbutton.h
    public/control_slickgroupbox.h
    public/control_slickprogress.h
    public/gui.h
    public/gui_palette.h
    public/keycodes.h
    public/screen.h
)

mark3_add_library(gui ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(gui 
    PUBLIC
        public        
    )
    
target_link_libraries(gui
    graphics
    heap
    mark3
    memutil    
    random
)
    