
#ifndef SCIMARK2_H
#define SCIMARK2_H

#define VERSION 2.0

#ifndef NULL 
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* benchmark types */

const  int BENCH_ALL = 0;  /* default option: run all benchmarks */
const  int BENCH_FFT = 1;
const  int BENCH_SOR = 2;
const  int BENCH_MONTE =3;
const  int BENCH_SPARSE = 4;
const  int BENCH_LU = 5;

#endif

