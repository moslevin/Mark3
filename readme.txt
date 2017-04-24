The Mark3 Real-time Operating System

Introduction

Mark3 is a real-time development platform for AVR, MSP430, and ARM Cortex-M series microcontrollers written using C++.  It features a fully-featured RTOS kernel, device drivers, and middleware, as well as a suite of examples and unit tests.  

Due to being written in C++ for AVR using the GCC toolchain, it also integrates directly into Arduino, without additional modifications to the source.
Directory Layout

arduino		Scripts and staging directory for exporting Mark3 for Arduino
bootloader	Source and makefiles for the Mark3 Bootloader
build		Platform/variant/toolchain specific build configuration files 
docs        Full PDF and HTML documentaiton
drivers		Device driver libraries
examples    Application code examples
export		Source-code export folder, used by export scripts
fonts		Fonts converted from TTF to bitmapped, C++ library fonts 
kernel		Main RTOS kernel code
libs        Optional support libraries and middleware
scripts		Build and test script folder
stage		Directory where binaries/headers are published at build
tests		Unit testing framework
util        Utility programs

Building from source using the make-based build system:

To build from source, the Mark3 build system requires the following:

	A supported toolchain (i.e. gcc-avr, arm-none-eabi-gcc)
	make support

For example, on debian-based distributions, such as Ubuntu, the avr toolchain can be installed using:

	apt-get install avr-libc gcc-avr

On Windows, the toolchain is provided as part of AVRStudio.  Please see the “Build System” section of the docs for instructions on configuring the system on Windows.

Once a sane build environment has been created, the kernel, libraries, examples and tests can be built by running ./scripts/build.sh from the root directory.  By default, Mark3 builds for the atmega328p target, but the target can be selected by manually configuring environment variables, or by running the ./scripts/set_target.sh script as follows:

	. ./scripts/set_target.sh <architecture> <variant> <toolchain>

	Where: 
	 <architecture> is the target CPU architecture(i.e. avr, msp430, cm0, cm3, cm4f)
	 <variant>		is the part name (i.e. atmega328p, msp430f2274, generic)
	 <toolchain>	is the build toolchain (i.e. gcc)

For example, to build the kernel for a generic ARM Cortex-M0 using a pre-configured arm-none-eabi-gcc toolchain, one would run the following commands:

	. ./scripts/set_target.sh cm0 generic gcc
	./scripts/build.sh

Building from source using the CMake-based build-system:

To build via CMake, a user requires a suitable, supported toolchain (i.e. gcc-avr, arm-none-eabi-gcc),
 CMake 3.4.2 or higher, and a backend supported by CMake (i.e. Ninja build)

The build can be configured by creating an output folder:

    mkdir kbuild

And then initializing the build system withing that folder:

    export ROOT_DIR=$(pwd)
    cd kbuild
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../mark3.cmake -Dmark3_arch=<cpu type> -Dmark3_variant=<variant> \ 
             -Dmark3_toolchain=<toolchain type> -Dmark3_root_dir=<full path to project directory> -GNinja        
    
The full build can be run by executing the backend build command:

    ninja 
    
To build just the kernel, build the following target

    ninja mark3

Note that not all targets will build in all kernel configurations.
    
Supported targets:

Currently, Mark3 supports the following parts:

	atmega328p
	atmega644
	atmega1284p
	atxmega256a3 (*experimental)
	atmega1280
	atmega2560
	msp430f2274
	ARM Cortex-M0 (generic)
	ARM Cortex-M3 (generic)
	ARM Cortex-M4 (generic, floating point)
	samd20 (cortex M0)

Additional Documentation

Please see the doxygen documentation in the ./docs folder for more information.   A lot of work has gone into documenting the project, and that's the best place to start if you have any questions.  The code examples are fairly comprehensive (as are the unit tests), so these should be referenced as necessary.  And of course, the source is very well-documented, so don't be afraid to browse through it.

Contact
Email Funkenstein Software for any additional queries or comments:
	info@mark3os.com

The official website for the project is located at:
	http://mark3os.com
