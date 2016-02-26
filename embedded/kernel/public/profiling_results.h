/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Wed Feb 24 20:33:50 EST 2016
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
    - Semaphore Initialization: 29 cycles (averaged over 84 iterations)
    - Semaphore Post (uncontested): 101 cycles (averaged over 84 iterations)
    - Semaphore Pend (uncontested): 60 cycles (averaged over 84 iterations)
    - Semaphore Flyback Time (Contested Pend): 1773 cycles (averaged over 84 iterations)
    - Mutex Init: 213 cycles (averaged over 84 iterations)
    - Mutex Claim: 229 cycles (averaged over 84 iterations)
    - Mutex Release: 133 cycles (averaged over 84 iterations)
    - Thread Initialize: 8365 cycles (averaged over 84 iterations)
    - Thread Start: 887 cycles (averaged over 84 iterations)
    - Context Switch: 158 cycles (averaged over 83 iterations)
    - Thread Schedule: 117 cycles (averaged over 83 iterations)
    . 
*/
