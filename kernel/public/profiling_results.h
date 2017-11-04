/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Wed Jun 28 23:26:13 EDT 2017
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
@code

Using built-in specs.
Reading specs from c:/program files (x86)/atmel/studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin/../lib/gcc/avr/4.9.2/device-specs/specs-avr2
COLLECT_GCC=C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe
COLLECT_LTO_WRAPPER=c:/program\ files\ (x86)/atmel/studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin/../libexec/gcc/avr/4.9.2/lto-wrapper.exe
Target: avr
Configured with: /home/toolsbuild/workspace/avr8-gnu-toolchain/src/gcc/configure LDFLAGS=-L/home/toolsbuild/workspace/avr8-gnu-toolchain/avr8-gnu-toolchain-win32_x86-hostlibs/lib CPPFLAGS= --target=avr --host=i686-w64-mingw32 --build=x86_64-pc-linux-gnu --prefix=/home/toolsbuild/workspace/avr8-gnu-toolchain/avr8-gnu-toolchain-win32_x86 --libdir=/home/toolsbuild/workspace/avr8-gnu-toolchain/avr8-gnu-toolchain-win32_x86/lib --enable-languages=c,c++ --with-dwarf2 --enable-doc --disable-shared --disable-libada --disable-libssp --disable-nls --with-avrlibc=yes --with-mpfr=/home/toolsbuild/workspace/avr8-gnu-toolchain/avr8-gnu-toolchain-win32_x86-hostlibs --with-gmp=/home/toolsbuild/workspace/avr8-gnu-toolchain/avr8-gnu-toolchain-win32_x86-hostlibs --with-mpc=/home/toolsbuild/workspace/avr8-gnu-toolchain/avr8-gnu-toolchain-win32_x86-hostlibs --enable-win32-registry=avrtoolchain --with-pkgversion=AVR_8_bit_GNU_Toolchain_3.5.4_1709 --with-bugurl=http://www.atmel.com
Thread model: single
gcc version 4.9.2 (AVR_8_bit_GNU_Toolchain_3.5.4_1709) 
@endcode

\section PROFRES Profiling Results
@code
    - Semaphore Initialization:  56 cycles (averaged over 100 iterations)
    - Semaphore Post (uncontested):  184 cycles (averaged over 100 iterations)
    - Semaphore Pend (uncontested):  96 cycles (averaged over 100 iterations)
    - Semaphore Flyback Time (Contested Pend):  2376 cycles (averaged over 100 iterations)
    - Mutex Init:  32 cycles (averaged over 100 iterations)
    - Mutex Claim:  200 cycles (averaged over 100 iterations)
    - Mutex Release:  128 cycles (averaged over 100 iterations)
    - Thread Initialize:  10808 cycles (averaged over 100 iterations)
    - Thread Start:  1176 cycles (averaged over 100 iterations)
    - Context Switch:  176 cycles (averaged over 100 iterations)
    - Thread Schedule:  72 cycles (averaged over 100 iterations)
@endcode
*/
