# Include common prelude make file
include $(ROOT_DIR)base.mak

# If we're building a library, set IS_LIB and LIBNAME
# If we're building a driver, set IS_DRV and DRVNAME
# If we're building an app, set IS_APP and APPNAME
IS_APP=1
APPNAME=mark3no

#this is the list of the objects required to build the kernel
CPP_SOURCE= main.cpp \
            nlfs_eeprom.cpp \
            led.cpp \
            button.cpp \
            bsp_buttons.cpp \
            bsp_heap.cpp \
            bsp_leds.cpp \
            bsp_rtc.cpp \
            bsp_tracelogger.cpp \
            bsp_uarts.cpp


LIBS=mark3 drvUART rtc memutil nlfs heap

# Include the rest of the script that is actually used for building the 
# outputs
include $(ROOT_DIR)build.mak
