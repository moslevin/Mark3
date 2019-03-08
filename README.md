
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/1c6a3d96c7cd471189dc75850cde71ba)](https://app.codacy.com/app/moslevin/Mark3?utm_source=github.com&utm_medium=referral&utm_content=moslevin/Mark3&utm_campaign=Badge_Grade_Dashboard)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/17835/badge.svg)](https://scan.coverity.com/projects/moslevin-mark3)
[![Sonarcloud Status](https://sonarcloud.io/api/project_badges/measure?project=moslevin_Mark3&metric=alert_status)](https://sonarcloud.io/project/issues?id=moslevin_Mark3)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://moslevin.github.io/Mark3)

The Mark3 Real-time Operating System

Introduction

Mark3 is a real-time development platform for AVR, MSP430, and ARM Cortex-M series microcontrollers written using C++.
It features a fully-featured RTOS kernel, device drivers, and middleware, as well as a suite of examples and unit tests.

Directory Structure

bsp         -   Board support packages, including drivers, for specific targets
build       -   Build system support for specific targets, scripts for environment configuration, test + profiling scripts
drivers     -   Device driver framework + common drivers.
external    -   3rd party (open source) libraries
kernel      -   RTOS kernel and base libraries
out         -   Build output directory
tools       -   Precompiled tools - simulators, toolchains, utilities
vendor      -   Proprietary 3rd-party software components for specific targets

To build via CMake, a user requires a suitable, supported toolchain (i.e. gcc-avr, arm-none-eabi-gcc),
CMake 3.4.2 or higher, and a backend supported by CMake (i.e. Ninja build).

For example, on debian-based distributions, such as Ubuntu, the avr toolchain can be installed using:

        apt-get install avr-libc gcc-avr cmake ninja-build

Once a sane build environment has been created, the kernel, libraries, examples and tests can be built
by running `ninja` from the target's build directory (/out/<target>/).  By default, Mark3 builds for the
atmega328p target, but the target can be selected by manually configuring environment variables, or
by running the ./build/set_target.sh script as follows:

        ./build/set_target.sh <architecture> <variant> <toolchain>

	Where: 
         <architecture> is the target CPU architecture(i.e. avr, msp430, cm0, cm3, cm4f)
         <variant>      is the part name (i.e. atmega328p, msp430f2274, generic)
         <toolchain>    is the build toolchain (i.e. gcc)

This script is a thin wrapper for the cmake configuration commands, and clears the output directory before
re-initializing cmake for the selected target.

To build the Mark3 kernel and middleware libraries for a generic ARM Cortex-M0 using a pre-configured
arm-none-eabi-gcc toolchain, one would run the following commands:

        ./build/set_target.sh cm0 generic gcc
        ./build/build.sh

To perform an incremental build, go into the cmake build directory (/out/<target/) and simply run 'ninja'.

Note that not all libraries/tests/examples will build in all kernel configurations.  The default kernel
configuration may need adjustment/tweaking to support a specific part.  See the documentation for details.
    
Supported targets:

Currently, Mark3 supports GCC toolchains for the following parts:

        atmega328p
        atmega644
        atmega1284p
        atxmega256a3 (*experimental)
        atmega1280
        atmega2560
        msp430f2274
        ARM Cortex-M0 (generic)
        ARM Cortex-M3 (generic)
        ARM Cortex-M4F (generic, hardware floating point)

Additional Documentation

Please see the doxygen documentation in the ./kernel/docs folder for more information.   A lot of work has gone
into documenting the project, and that's the best place to start if you have any questions.  The code examples
are fairly comprehensive (as are the unit tests), so these should be referenced as necessary.  And of course,
the source is very well-documented, so don't be afraid to browse through it.

Contact
Email Funkenstein Software for any additional queries or comments:
        funk_dev@hotmail.com

The official website for the project is located at:
        https://github.com/moslevin/Mark3
