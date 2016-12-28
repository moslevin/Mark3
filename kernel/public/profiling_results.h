/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Fri Sep 30 21:23:34 EDT 2016
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
@code

Using built-in specs.
COLLECT_GCC=avr-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/avr/4.8.2/lto-wrapper
Target: avr
Configured with: ../src/configure -v --enable-languages=c,c++ --prefix=/usr/lib --infodir=/usr/share/info --mandir=/usr/share/man --bindir=/usr/bin --libexecdir=/usr/lib --libdir=/usr/lib --enable-shared --with-system-zlib --enable-long-long --enable-nls --without-included-gettext --disable-libssp --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=avr
Thread model: single
gcc version 4.8.2 (GCC) 
@endcode

\section PROFRES Profiling Results
@code
    - Semaphore Initialization: 4123168596 cycles (averaged over 75 iterations)
    - Semaphore Post (uncontested): 121 cycles (averaged over 75 iterations)
    - Semaphore Pend (uncontested): 58 cycles (averaged over 75 iterations)
    - Semaphore Flyback Time (Contested Pend): 1760 cycles (averaged over 75 iterations)
    - Mutex Init: 240 cycles (averaged over 75 iterations)
    - Mutex Claim: 177 cycles (averaged over 75 iterations)
    - Mutex Release: 106 cycles (averaged over 75 iterations)
    - Thread Initialize: 8304 cycles (averaged over 75 iterations)
    - Thread Start: 824 cycles (averaged over 74 iterations)
    - Context Switch: 159 cycles (averaged over 74 iterations)
    - Thread Schedule: 88 cycles (averaged over 74 iterations)
@endcode
*/
