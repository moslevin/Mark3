/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Mon May  2 21:21:30 EDT 2016
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
Using built-in specs.
COLLECT_GCC=avr-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/avr/4.8.2/lto-wrapper
Target: avr
Configured with: ../src/configure -v --enable-languages=c,c++ --prefix=/usr/lib --infodir=/usr/share/info --mandir=/usr/share/man --bindir=/usr/bin --libexecdir=/usr/lib --libdir=/usr/lib --enable-shared --with-system-zlib --enable-long-long --enable-nls --without-included-gettext --disable-libssp --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=avr
Thread model: single
gcc version 4.8.2 (GCC) 
\endcode
\section PROFRES Profiling Results
    - Semaphore Initialization: 40 cycles (averaged over 85 iterations)
    - Semaphore Post (uncontested): 88 cycles (averaged over 85 iterations)
    - Semaphore Pend (uncontested): 79 cycles (averaged over 85 iterations)
    - Semaphore Flyback Time (Contested Pend): 1674 cycles (averaged over 85 iterations)
    - Mutex Init: 216 cycles (averaged over 86 iterations)
    - Mutex Claim: 184 cycles (averaged over 86 iterations)
    - Mutex Release: 135 cycles (averaged over 86 iterations)
    - Thread Initialize: 8272 cycles (averaged over 85 iterations)
    - Thread Start: 784 cycles (averaged over 85 iterations)
    - Context Switch: 176 cycles (averaged over 85 iterations)
    - Thread Schedule: 72 cycles (averaged over 85 iterations)
    . 
*/
