The Mark3 Real-time Operating System

Introduction

Mark3 is a real-time development platform for AVR, MSP430, and ARM Cortex-M series microcontrollers written using C++.  It features a fully-featured RTOS kernel, device drivers, and middleware, as well as a suite of examples and unit tests.  

Due to being written in C++ for AVR using the GCC toolchain, it also integrates directly into Arduino, without additional modifications to the source.
Directory Layout

arduino		Scripts and staging directory for exporting Mark3 for Arduino
bootloader	Source and makefiles for the Mark3 Bootloader
build		Platform/variant/toolchain specific build configuration files 
docs			PDF and HTML documentaiton
drivers		Device driver libraries
examples		Application code examples
export		Source-code export folder, used by export scripts
fonts		Fonts converted from TTF to bitmapped, C++ library fonts 
kernel		Main RTOS kernel code
libs			Optional support libraries and middleware
scripts		Build and test script folder
stage		Directory where binaries/headers are published at build
tests		Unit testing framework
util			Utility programs

Building the source

To build the source, the Mark3 build system requires the following:

	avr-gcc toolchain
	make support

On debian-based distributions, such as Ubuntu, the avr toolchain can be installed using:

	apt-get install avr-libc gcc-avr

On Windows, the toolchain is provided as part of AVRStudio.  Please see the “Build System” section of the docs for instructions on configuring the system on Windows.

Once a sane build environment has been created, the kernel, libraries, examples and tests can be built by running ./scripts/build.sh from the root directory.  By default, Mark3 builds for the atmega328p target, although other supported targets can be configured through environment variables.  See the base.mak makefile, and “Building the Kernel” in the docs for more information on configuring these variables.

Supported targets

Currently, Mark3 supports the following parts:

	atmega328p
	atmega644
	atmega1284p
	atxmega256a3 (*experimental)
	msp430f2274
	samd20 (cortex M0)
	stm32f0 (cortex M0)

Additional Documentation

Please see the doxygen documentation in the ./docs folder for more information.   A lot of work has gone into documenting the project, and that's the best place to start if you have any questions.  The code examples are fairly comprehensive (as are the unit tests), so these should be referenced as necessary.  And of course, the source is very well-documented, so don't be afraid to browse through it.
Contact
Email Funkenstein Software for any additional queries or comments:
	info@mark3os.com

The official website for the project is located at:
	http://mark3os.com