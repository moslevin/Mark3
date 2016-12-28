# Include common prelude make file
include $(ROOT_DIR)base.mak

# If we're building a library, set IS_LIB and LIBNAME
# If we're building a driver, set IS_DRV and DRVNAME
# If we're building an app, set IS_APP and APPNAME
IS_APP=1
APPNAME=brickbreak

#this is the list of the objects required to build the kernel
CPP_SOURCE= brball.cpp \
	brcommon.cpp \
	breakout.cpp \
	brlevel.cpp \
	brlives.cpp \
	brpaddle.cpp \
	brplayfield.cpp \
	brscore.cpp \
	font_8.cpp \
	highscore.cpp \
	markade2.cpp

LIBS=mark3 drvSound drvFlavrJoy drvBasicJoy joystick drvGraphicsFlavr drvGraphicsST7735 graphics memutil

# Include the rest of the script that is actually used for building the 
# outputs
include $(ROOT_DIR)build.mak
