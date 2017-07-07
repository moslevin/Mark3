/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Fri  7 Jul 01:40:14 UTC 2017
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
@code

Using built-in specs.
COLLECT_GCC=avr-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/avr/4.8.1/lto-wrapper
Target: avr
Configured with: ../src/configure -v --enable-languages=c,c++ --prefix=/usr/lib --infodir=/usr/share/info --mandir=/usr/share/man --bindir=/usr/bin --libexecdir=/usr/lib --libdir=/usr/lib --enable-shared --with-system-zlib --enable-long-long --enable-nls --without-included-gettext --disable-libssp --build=arm-linux-gnueabihf --host=arm-linux-gnueabihf --target=avr
Thread model: single
gcc version 4.8.1 (GCC) 
@endcode

\section PROFRES Profiling Results
@code
    - Semaphore Initialization:  40 cycles (averaged over 100 iterations)
    - Semaphore Post (uncontested):  120 cycles (averaged over 100 iterations)
    - Semaphore Pend (uncontested):  32 cycles (averaged over 100 iterations)
    - Semaphore Flyback Time (Contested Pend):  2272 cycles (averaged over 100 iterations)
    - Mutex Init:  000004294967272 cycles (averaged over 100 iterations)
    - Mutex Claim:  216 cycles (averaged over 100 iterations)
    - Mutex Release:  120 cycles (averaged over 100 iterations)
    - Thread Initialize:  8312 cycles (averaged over 100 iterations)
    - Thread Start:  1096 cycles (averaged over 100 iterations)
    - Context Switch:  168 cycles (averaged over 100 iterations)
    - Thread Schedule:  32 cycles (averaged over 100 iterations)
@endcode
*/
