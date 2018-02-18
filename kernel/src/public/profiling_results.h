/*!
\page PROFILERES Profiling Results
The following profiling results were obtained using an ATMega328p @ 16MHz.
The test cases are designed to make use of the kernel profiler, which accurately
measures the performance of the fundamental system APIs, in order to provide 
information for user comparison, as well as to ensure that regressions are not
being introduced into the system.

\section PROFILEATE Date Performed 
Fri Feb 16 23:56:23 STD 2018
\section COMPVER Compiler Information 
The kernel and test code used in these results were built using the following compiler:
@code

Using built-in specs.
Reading specs from /usr/lib/gcc/avr/4.9.2/device-specs/specs-avr2
COLLECT_GCC=avr-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/avr/4.9.2/lto-wrapper
Target: avr
Configured with: ../src/configure -v --enable-languages=c,c++ --prefix=/usr/lib --infodir=/usr/share/info --mandir=/usr/share/man --bindir=/usr/bin --libexecdir=/usr/lib --libdir=/usr/lib --enable-shared --with-system-zlib --enable-long-long --enable-nls --without-included-gettext --disable-libssp --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=avr CFLAGS='-g -O2 -fstack-protector-strong -Wformat ' CPPFLAGS=-D_FORTIFY_SOURCE=2 CXXFLAGS='-g -O2 -fstack-protector-strong -Wformat ' FCFLAGS='-g -O2 -fstack-protector-strong' FFLAGS='-g -O2 -fstack-protector-strong' GCJFLAGS='-g -O2 -fstack-protector-strong' LDFLAGS='-Wl,-Bsymbolic-functions -Wl,-z,relro' OBJCFLAGS='-g -O2 -fstack-protector-strong -Wformat ' OBJCXXFLAGS='-g -O2 -fstack-protector-strong -Wformat '
Thread model: single
gcc version 4.9.2 (GCC) 
@endcode

\section PROFRES Profiling Results
@code
    - Semaphore Initialization:  cycles (averaged over 100 iterations)
    - Semaphore Post (uncontested):  cycles (averaged over 100 iterations)
    - Semaphore Pend (uncontested):  cycles (averaged over 100 iterations)
    - Semaphore Flyback Time (Contested Pend):  cycles (averaged over 100 iterations)
    - Mutex Init:  cycles (averaged over 100 iterations)
