3/8/95
This is the README file for the version 2 distribution of BYTEmark (tm), BYTE Magazine's Native Mode Benchmarks.  On this disk, you will find:

Directory: DOCS
    BDOC.RTF - Benchmark documentation in RTF format.
    BDOC.TXT - Benchmark documentation in ASCII text.

Directory: SOURCE
    Source files for the benchmarks.  You will also find the
    file NNET.DAT.  This is the input data file for the neural
    net simulation test, and must be in the subdirectory with
    the executable when you run the benchmark.

Directory: DOS
    Executables for MS-DOS.  Optimized for the Pentium.
    Runs with the Rational System's DOS Extender provided
    with Watcom's C/C++ version 10.0 compiler.  Compiled with
    Watcom C/C++ compiler.

Directory: WINDOWS
    Executable for Windows 3.1.  Optimized for the Pentium.
    Compiled with Watcom C/C++ version 10.0 compiler.

Directory: NT\INTEL
    Executable for Windows NT.  Optimized for the Pentium.
    Compiled with Watcom C/C++ version 10.0 compiler.

Directory: NT\MIPS
    Executable for Windows NT, MIPS processor.  Optimized for
    fastest code.  Compiled with Microsoft Visual C/C++ 2.0.

Directory: NT\ALPHA
    Executable for Windows NT, Alpha processor.  Optimized for
    fastest code.  Compiled with Microsoft Visual C/C++ 2.0.

Versions for other operating systems and processor platforms should be available soon.  Contact BYTE Magazine for updates to the benchmarks.

Please read the following DISCLAIMER, USAGE, and REPORTING topics.  You should also read the documentation in the DOCS sudirectory before working with the benchmarks.  If you have any questions, contact BYTE Magazine at (603) 924-9281 and ask for the BYTE Lab.

DISCLAIMER

1. The source, executable, and documentation files that comprise the BYTEmark benchmarks are made available on an "as is" basis.  This means that we at BYTE Magazine have made every reasonable effort to verify that the there are no errors in the source and executable code.  We cannot, however, guarantee that the programs are error-free.  Consequently, McGraw-HIll and BYTE Magazine make no claims in regard to the fitness of the source code, executable code, and documentation of the BYTEmark.  Furthermore, BYTE Magazine, McGraw-Hill, and all employees of McGraw-Hill cannot be held responsible for any damages resulting from the use of this code or the results obtained from using this code.

USAGE

1. The source and executables for these benchmarks may be distributed freely, as long as a) the files remain unaltered and b) all the supporting documentation files are included in the distribution.

2. Modifications to the code (beyond activating or deactivating #define statements, #debug statements, or similar conditional compilation statements) will be made by BYTE Magazine only.  Suggested modifications should be submitted to the BYTE Lab. (Land mail such suggestions to: Byte Magazine, One Phoenix Mill Ln, Peterborough, NH, 03458, Attn: BYTE Lab.)

REPORTING RESULTS

You may report or otherwise publish results obtained from these benchmarks subject to the following requirements.

1. The executables you use to generate the results are a) original executables obtained from BYTE or b) compiled from UNALTERED source code.  (In case (b), the term UNALTERED means source code that has not been changed beyond activating or deactivating conditional compilation statements such as a "#define" directive.)

2. In case 1 (a) above -- reporting results from original executables -- the following must be reported with the results: a) the version number of the benchmark and b) the operating system name and version used.

3. In case 1 (b) above -- reporting results from an executable generated from a compilation -- the following must be reported with the results: a) version number of the benchmark, b), the operating system name and version used, and c) compiler and compiler version.  Additionally, if you publish these results, you must make available the compiler and linker switch settings to ANYONE who requests that information.

4. In all cases, if you publish the BYTEmark results, you must make available the following to ANYONE who requests: a) system model name (or whatever means uniquely identifies the system tested), b) CPU model and clock speed, c) FPU model and clock speed, d) amount of primary and secondary cache, e) amount of system memory, f) clock speed of memory bus (if different from CPU clock speed), and g) date the benchmark was run.

Any questions regarding the above should be directed to the BYTE Lab at BYTE Magazine (see the address given above).

--Rick Grehan
BYTE Magazine
