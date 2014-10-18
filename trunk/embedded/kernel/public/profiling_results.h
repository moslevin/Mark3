/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Fri Oct 17 21:37:19 EDT 2014
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
./scripts/profile_sim.sh: 50: ./scripts/profile_sim.sh: /home/moslevin/atmel/bin/avr-gcc: not found
\endcode
\section PROFRES Profiling Results
    - Semaphore Initialization: 17 cycles (averaged over 84 iterations)
    - Semaphore Post (uncontested): 983 cycles (averaged over 84 iterations)
    - Semaphore Pend (uncontested): 849 cycles (averaged over 83 iterations)
    - Semaphore Flyback Time (Contested Pend): 3337 cycles (averaged over 83 iterations)
    - Mutex Init: 393 cycles (averaged over 84 iterations)
    - Mutex Claim: 108 cycles (averaged over 84 iterations)
    - Mutex Release: 873 cycles (averaged over 84 iterations)
    - Thread Initialize: 8217 cycles (averaged over 83 iterations)
    - Thread Start: 761 cycles (averaged over 83 iterations)
    - Context Switch: 161 cycles (averaged over 83 iterations)
    - Thread Schedule: 73 cycles (averaged over 83 iterations)
    . 
*/
