# Include common prelude make file
include $(ROOT_DIR)base.mak

# If we're building a library, set IS_LIB and LIBNAME
# If we're building a driver, set IS_DRV and DRVNAME
# If we're building an app, set IS_APP and APPNAME
IS_LIB=1
LIBNAME=gui

#this is the list of the objects required to build the kernel
CPP_SOURCE=gui.cpp \
 screen.cpp \
 control_brushpanel.cpp \
 control_button.cpp \
 control_checkbox.cpp \
 control_gamepanel.cpp \
 control_groupbox.cpp \
 control_label.cpp \
 control_notification.cpp \
 control_panel.cpp \
 control_progress.cpp \
 control_slickbutton.cpp \
 control_slickgroupbox.cpp \
 control_slickprogress.cpp \
 control_sevenseg.cpp

# Include the rest of the script that is actually used for building the 
# outputs
include $(ROOT_DIR)build.mak
