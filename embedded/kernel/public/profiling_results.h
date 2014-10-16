/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Wed Oct 15 22:33:30 EDT 2014
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
./scripts/profile_sim.sh: 50: ./scripts/profile_sim.sh: /home/moslevin/atmel/bin/avr-gcc: not found
\endcode
\section PROFRES Profiling Results
    - Semaphore Initialization: 24 cycles (averaged over 124 iterations)
    - Semaphore Post (uncontested): 975 cycles (averaged over 124 iterations)
    - Semaphore Pend (uncontested): 856 cycles (averaged over 124 iterations)
    - Semaphore Flyback Time (Contested Pend): 3413 cycles (averaged over 124 iterations)
    - Mutex Init: 405 cycles (averaged over 125 iterations)
    - Mutex Claim: 1040 cycles (averaged over 124 iterations)
    - Mutex Release: 832 cycles (averaged over 124 iterations)
    - Thread Initialize: 8248 cycles (averaged over 124 iterations)
    - Thread Start: 760 cycles (averaged over 124 iterations)
    - Context Switch: 168 cycles (averaged over 124 iterations)
    - Thread Schedule: 32 cycles (averaged over 124 iterations)
    . 
*/
