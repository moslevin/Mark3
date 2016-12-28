# Include common prelude make file
include $(ROOT_DIR)base.mak

# If we're building a library, set IS_LIB and LIBNAME
# If we're building a driver, set IS_DRV and DRVNAME
# If we're building an app, set IS_APP and APPNAME
IS_APP=1
APPNAME=lab1_kernel_setup

# Board specific configuration:
# - Add board-specific linker script location to the linker flags;
#   This is typically provided by a vendor as part of the BSP, and is
#   not part of Mark3 itself.  The path provided is an example of where
#   such a file *could* live, and not a reference to an actual file
#   in the distribution.
LFLAGS+=-T$(ROOT_DIR)/ext/f4/startup/Linker.ld

#this is the list of the objects required to build the kernel
CPP_SOURCE=main.cpp

# Board specific configuration:
# - Link against board-specific libraries, in addition to the Mark3 kernel.
# - In this example, it is assumed that 2 libraries, libstartup and libstdperiph
#   are built through the Mark3 build system, and available to link against this
#   application.  Note that these are examples only, and not a reference to
#   actual files in the distribution.
LIBS=mark3 startup stdperiph

# Include the rest of the script that is actually used for building the 
# outputs
include $(ROOT_DIR)build.mak
