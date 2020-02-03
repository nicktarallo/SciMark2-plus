# SciMark2-plus
This is an improved version of the SciMark 2.0 benchmark, originally developed by [Roldan Pozo](https://math.nist.gov/~RPozo/) and [Bruce Miller](https://math.nist.gov/~BMiller/). This is based on source code downloaded from the [official webpage](https://math.nist.gov/scimark2/download_c.html) on 2020-02-03.

For general usage, use the `-h` or `--help` option:
<pre>
Usage: scimark2 [options]

Perform several benchmarks for scientific and numerical computing.

Options:
  -b, --benchmark=BENCH    Run any of the following benchmarks:
                             all:    Run all subtests below (default)
                             fft:    Fast Fourier Transform
                             sor:    Jacobi Successive Over-relaxation
                             monte:  Monte Carlo integration
                             sparse: Sparse matrix multiply
                             lu:     Dense LU matrix factorization
  -l, --large              Use data structures that do not typically fit in
                             cache
  -t, --mintime=NUM        Specify minimum length of time in seconds to run
                             each benchmark
  -h, --help               Display this help and exit
</pre>

For steps on building this benchmark, see below for a modified version of the original description taken from the [official webpage](https://math.nist.gov/scimark2/download_c.html).

---

This is an ANSI C version of the SciMark2 benchmark, translated from the original Java sources. The intent in making this benchmark available in C is mainly for performance comparisons.

[Version 4.0](https://math.nist.gov/scimark2/scimark4c.zip) of the software has added check-sum counters to adequately test newer optimizing compilers.

Results of this benchmark can be sent to pozo@nist.gov.

The program is split up into the main driver (scimark2.c) and kernel routines. A sample makefile is included; however, one could simply write

> cc -o scimark2  -O *.c

and then run

> scimark2

This produces an output similar to

```text
**                                                              **
** SciMark2 Numeric Benchmark, see http://math.nist.gov/scimark **
** for details. (Results can be submitted to pozo@nist.gov)     **
**                                                              **
Using       2.00 seconds min time per kenel.
FFT             Mflops:    63.38    (N=1024)
SOR             Mflops:   124.80    (100 x 100)
MonteCarlo:     Mflops:    16.05
Sparse matmult  Mflops:    59.15    (N=1000, nz=5000)
LU              Mflops:    64.40    (M=100, N=100)
Composite Score:           65.56
```

The first SciMark number reported is the composite score, followed by the an approximate Mflop rate for each kernel.

To run the "large" version of this benchmark (with data structures that typically do not fit in cache) use

>scimark2 --large
