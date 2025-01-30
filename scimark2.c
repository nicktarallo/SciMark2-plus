#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "Random.h"
#include "kernel.h"
#include "constants.h"
#include "scimark2.h"

static struct option const long_options[] =
{
    {"benchmark", required_argument, NULL, 'b'},
    {"help", no_argument, NULL, 'h'},
    {"large", no_argument, NULL, 'l'},
    {"mintime", required_argument, NULL, 't' },
    {NULL, 0, NULL, 0}
};

void usage ()
{
    fputs ("Usage: scimark2 [options]\n\n", stdout);
    fputs ("\
Perform several benchmarks for scientific and numerical computing.\n\n\
Options:\n\
  -b, --benchmark=BENCH    Run any of the following benchmarks:\n\
                             all:    Run all subtests below (default)\n\
                             fft:    Fast Fourier Transform\n\
                             sor:    Jacobi Successive Over-relaxation\n\
                             monte:  Monte Carlo integration\n\
                             sparse: Sparse matrix multiply\n\
                             lu:     Dense LU matrix factorization\n\
  -l, --large              Use data structures that do not typically fit in\n\
                             cache\n\
  -t, --mintime=NUM        Specify minimum length of time in seconds to run\n\
                             each benchmark\n\
  -h, --help               Display this help and exit\n", stdout);
    return;
}

void print_banner ()
{
 printf("**                                                              **\n");
 printf("** SciMark2 Numeric Benchmark, see http://math.nist.gov/scimark **\n");
 printf("** for details. (Results can be submitted to pozo@nist.gov)     **\n");
 printf("**                                                              **\n");
}

int main (int argc, char *argv[])
{
    /* default to the (small) cache-contained version */

    double min_time = RESOLUTION_DEFAULT;

    int FFT_size = FFT_SIZE;
    int SOR_size =  SOR_SIZE;
    int Sparse_size_M = SPARSE_SIZE_M;
    int Sparse_size_nz = SPARSE_SIZE_nz;
    int LU_size = LU_SIZE;
    int bench_type = BENCH_ALL;

    double res[6] = {0.0};
    Random R = new_Random_seed(RANDOM_SEED);

    /* parse options */
    int c;
    while ((c = getopt_long (argc, argv, "b:lt:h", long_options, NULL)) 
            != -1)
    {
        switch (c)
        {
            case 'b':
                if (strcmp(optarg, "all") == 0) bench_type = BENCH_ALL;
                else if (strcmp(optarg, "fft") == 0) bench_type = BENCH_FFT;
                else if (strcmp(optarg, "sor") == 0) bench_type = BENCH_SOR;
                else if (strcmp(optarg, "monte") == 0) bench_type = BENCH_MONTE;
                else if (strcmp(optarg, "sparse") == 0) 
                    bench_type = BENCH_SPARSE;
                else if (strcmp(optarg, "lu") == 0) bench_type = BENCH_LU;
                else {
                    printf("Benchmark \"%s\" not recognized. Options are \
[all, fft, sor, monte, sparse, lu].\n", optarg);
                    return 1;
                }
                printf("Running benchmark: %s\n", optarg);
                break;

            case 'l':
                fputs("Running in large mode\n", stdout);
                FFT_size = LG_FFT_SIZE;
                SOR_size = LG_SOR_SIZE;
                Sparse_size_M = LG_SPARSE_SIZE_M;
                Sparse_size_nz = LG_SPARSE_SIZE_nz;
                LU_size = LG_LU_SIZE;
                break;

            case 't':
                min_time = atof(optarg);
                break;

            case 'h':
                usage();
                return 0;
  
            default:
                usage();
                return 1;
        }
    }

    /* run the benchmark */
    print_banner();
    /* We are not using a min time per kernel for the sparse matrix multiplication */
    /* printf("Using %10.2f seconds min time per kernel.\n", min_time); */

    if (bench_type == BENCH_FFT || bench_type == BENCH_ALL) 
    {
        res[1] = kernel_measureFFT( FFT_size, min_time, R);
        printf("FFT             Mflops: %8.2f    (N=%d)\n", res[1],
                FFT_size);
    }
    if (bench_type == BENCH_SOR || bench_type == BENCH_ALL) 
    {
        res[2] = kernel_measureSOR( SOR_size, min_time, R);
        printf("SOR             Mflops: %8.2f    (%d x %d)\n", res[2], 
                SOR_size, SOR_size);
    }
    if (bench_type == BENCH_MONTE || bench_type == BENCH_ALL) 
    {
        res[3] = kernel_measureMonteCarlo(min_time, R);
        printf("MonteCarlo:     Mflops: %8.2f\n", res[3]);
    }
    if (bench_type == BENCH_SPARSE || bench_type == BENCH_ALL) 
    {
	int iterations = 10;
	double total_time = 0;
	/* Add looping to run 10 times */
	int i;
	double time_on_loop;
	for (i = 0; i < 10; i++) {
	    res[4] = kernel_measureSparseMatMult( Sparse_size_M,
                Sparse_size_nz, min_time, R, &time_on_loop);
            printf("Sparse matmult  Mflops: %8.2f    (N=%d, nz=%d)\n", res[4],
                Sparse_size_M, Sparse_size_nz);
	    total_time += time_on_loop;
	} 
	double average_time = total_time / ((double) iterations);
	printf("\nAverage time per 1M: %lf\n", average_time);
        
    }
    if (bench_type == BENCH_LU || bench_type == BENCH_ALL) 
    {
        res[5] = kernel_measureLU( LU_size, min_time, R);
        printf("LU              Mflops: %8.2f    (M=%d, N=%d)\n", res[5],
                LU_size, LU_size);
    }
    if (bench_type == BENCH_ALL) 
    {
        res[0] = (res[1] + res[2] + res[3] + res[4] + res[5]) / 5;
        printf("Composite Score:        %8.2f\n" ,res[0]);
    }

    Random_delete(R);

    return 0;  
}
