/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Sat Jun  1 10:43:06 EDT 2013
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
./profile.sh: 55: ./profile.sh: /home/moslevin/atmel/bin/avr-gcc: not found
\endcode
\section PROFRES Profiling Results
    - Semaphore Initialization: 7 cycles (averaged over 83 iterations)
    - Semaphore Post (uncontested): 180 cycles (averaged over 83 iterations)
    - Semaphore Pend (uncontested): 67 cycles (averaged over 83 iterations)
    - Semaphore Flyback Time (Contested Pend): 1553 cycles (averaged over 83 iterations)
    - Mutex Init: 0 cycles (averaged over 83 iterations)
    - Mutex Claim: 143 cycles (averaged over 83 iterations)
    - Mutex Release: 49 cycles (averaged over 83 iterations)
    - Thread Initialize: 7800 cycles (averaged over 83 iterations)
    - Thread Start: 803 cycles (averaged over 83 iterations)
    - Context Switch: 198 cycles (averaged over 83 iterations)
    - Thread Schedule: 47 cycles (averaged over 83 iterations)
    . 
*/
