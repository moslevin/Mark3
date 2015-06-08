/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Wed May 27 21:18:21 EDT 2015
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
./scripts/profile.sh: 55: ./scripts/profile.sh: /home/moslevin/atmel/bin/avr-gcc: not found
\endcode
\section PROFRES Profiling Results
    - Semaphore Initialization:  cycles (averaged over 0 iterations)
    - Semaphore Post (uncontested):  cycles (averaged over 0 iterations)
    - Semaphore Pend (uncontested):  cycles (averaged over 0 iterations)
    - Semaphore Flyback Time (Contested Pend):  cycles (averaged over 0 iterations)
    - Mutex Init:  cycles (averaged over 0 iterations)
    - Mutex Claim:  cycles (averaged over 0 iterations)
    - Mutex Release:  cycles (averaged over 0 iterations)
    - Thread Initialize:  cycles (averaged over 0 iterations)
    - Thread Start:  cycles (averaged over 0 iterations)
    - Context Switch:  cycles (averaged over 0 iterations)
    - Thread Schedule:  cycles (averaged over 0 iterations)
    . 
*/
