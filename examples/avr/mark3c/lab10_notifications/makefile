# Include common prelude make file
include $(ROOT_DIR)base.mak

# If we're building a library, set IS_LIB and LIBNAME
# If we're building a driver, set IS_DRV and DRVNAME
# If we're building an app, set IS_APP and APPNAME
IS_APP=1
APPNAME=c_lab10_notifications

#this is the list of the objects required to build the kernel
C_SOURCE=main.c

LIBS=mark3c mark3

# Include the rest of the script that is actually used for building the 
# outputs
include $(ROOT_DIR)build.mak
