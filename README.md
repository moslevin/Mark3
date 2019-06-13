# The Mark3 Real-Time Operating System

_images/mark3_small.png

The Mark3 Realtime Kernel is a completely free, open-source, real-time operating system aimed at bringing powerful, easy-to-use multitasking to microcontroller systems without MMUs.

_images/banner2.png

The RTOS is written using a super portable design that scales to many common processor architectures, including a variety of 8, 16, 32, and 64-bit targets. The flexible CMake-based build system facilitates compiling the kernel, tests, examples, and user-application code for any supported target with a consistent interface.

The API is rich and surprisingly simple – with six function calls, you can set up the kernel, initialize two threads, and start the scheduler.

Written in modern C++, Mark3 makes use of modern language features that improve code quality, reduces duplication, and simplifies API usage. C++ isn’t your thing? No problem! there’s a complete set of C-language bindings available to facilitate the use of Mark3 in a wider variety of environments.

The Mark3 kernel releases contain zero compiler warnings, zero compiler errors, and have zero unit test failures. The build and test process can be automated through the Mark3-docker project, allowing for easy integration with continuous integration environments. The kernel is also run through static analysis tools, automated profiling and documentation tools.

The source is fully-documented with doxygen, and example code is provided to illustrate core concepts. The result is a performant RTOS, which is easy to read, easy to understand, and easy to extend to fit your needs.
