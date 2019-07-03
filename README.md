[![Codacy Badge](https://api.codacy.com/project/badge/Grade/1c6a3d96c7cd471189dc75850cde71ba)](https://app.codacy.com/app/moslevin/Mark3?utm_source=github.com&utm_medium=referral&utm_content=moslevin/Mark3&utm_campaign=Badge_Grade_Dashboard)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/17835/badge.svg)](https://scan.coverity.com/projects/moslevin-mark3)
[![Sonarcloud Status](https://sonarcloud.io/api/project_badges/measure?project=moslevin_Mark3&metric=alert_status)](https://sonarcloud.io/project/issues?id=moslevin_Mark3)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://moslevin.github.io/Mark3)

# The Mark3 Real-Time Operating System

![Mark3 Logo](https://github.com/moslevin/Mark3/raw/master/mark3_small.png "Mark3 Real Time Operating System")

The Mark3 Realtime Kernel is a completely free, open-source, real-time operating system aimed at bringing powerful, easy-to-use multitasking to microcontroller systems without MMUs.

![Mark3 Logo](https://github.com/moslevin/Mark3/raw/master/banner2.png "Features")

## Overview

The Mark3 RTOS is written using a super portable design that scales to many common processor architectures, including a variety of 8, 16, 32, and 64-bit targets. The flexible CMake-based build system facilitates compiling the kernel, tests, examples, and user-application code for any supported target with a consistent interface.

The API is rich and surprisingly simple – with six function calls, you can set up the kernel, initialize two threads, and start the scheduler.

Written in modern C++, Mark3 makes use of modern language features that improve code quality, reduces duplication, and simplifies API usage. C++ isn’t your thing? No problem! there’s a complete set of C-language bindings available to facilitate the use of Mark3 in a wider variety of environments.

The Mark3 kernel releases contain zero compiler warnings, zero compiler errors, and have zero unit test failures. The build and test process can be automated through the [Mark3 Docker Project](https://github.com/moslevin/mark3-docker) project, allowing for easy integration with continuous integration environments. The kernel is also run through static analysis tools, automated profiling and documentation tools.

The source is fully-documented with doxygen, and example code is provided to illustrate core concepts. The result is a performant RTOS, which is easy to read, easy to understand, and easy to extend to fit your needs.

## Quick Links

[Mark3 Kernel Documentation](https://moslevin.github.io/mark3-doc/)

[Mark3 OS Repo](https://github.com/moslevin/mark3-repo)

[Mark3 Docker Project](https://github.com/moslevin/mark3-docker)

