/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Sun Jan 27 10:36:50 EST 2013
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
./profile.sh: 55: ./profile.sh: /home/moslevin/atmel/bin/avr-gcc: not found
\endcode
\section PROFRES Profiling Results
    - Semaphore Initialization: 2356994244 cycles (averaged over 82 iterations)
    - Semaphore Post (uncontested): 148 cycles (averaged over 82 iterations)
    - Semaphore Pend (uncontested): 68 cycles (averaged over 82 iterations)
    - Semaphore Flyback Time (Contested Pend): 1668 cycles (averaged over 82 iterations)
    - Mutex Init: 4190211955 cycles (averaged over 82 iterations)
    - Mutex Claim: 204 cycles (averaged over 82 iterations)
    - Mutex Release: 47 cycles (averaged over 82 iterations)
    - Thread Initialize: 7897 cycles (averaged over 82 iterations)
    - Thread Start: 798 cycles (averaged over 82 iterations)
    - Context Switch: 171 cycles (averaged over 82 iterations)
    - Thread Schedule: 70 cycles (averaged over 82 iterations)
    . 
*/
