// k-means clustering on L0
// initial value is randomly generated
//
// dimention of points: 4
// number of points: N
// number of points each part: PSIZE
// k: K
// max number of iterations: MAX_ITER 

#ifndef KMEANS_COMMON_H
#define KMEANS_COMMON_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// #define DEBUG

// output centroids on finished
// #define OUTPUT_ON_FINISH
#define OUTPUT_ON_FINISH

#define PRINT_ITER

// dataset size: choose only one
// #define TINY_EXP
// #define SMALL_EXP
// #define LARGE_EXP

#define MAX_ITER (6)
// #define MAX_ITER (20)


#ifdef TINY_EXP
#define N (10)
#define PSIZE (10)
#define K (2)

#else
#ifdef LARGE_EXP
#define N (204800000)
#define PSIZE (640000)
#define K (1000)

#else
#ifdef SMALL_EXP
#define N (512 * 1024)
#define PSIZE (128 * 1024)
#define K (20)

#else
#define N (64 * 1024 * 1024)
#define PSIZE (512 * 1024)
#define K (20)
#endif
#endif
#endif


// whether enable local aggregation
// Note: must defined
#define LOCAL_AGGREG


// Note: do not change this
// get input from STDIN
#define INPUT_STDIN
// Note: to use INPUT_STDIN, the N/PSIZE should be the number of VPCs


// use libi0 libraries instead
#define STDOUT (0x100000208)

#define PMEM_IN_SIZE  (N * 4 * sizeof_uint64_t)
#define PMEM_OUT_SIZE (K * 4 * sizeof_uint64_t)

#define PMEM_IN_ADDR  PPM_START
#define PMEM_OUT_ADDR (PPM_START + N * sizeof_uint64_t)

// for kmeans-from-pmem 
// and kmeans-stdin-to-pmem
#ifdef FROM_PMEM
standalone long *pmem_in;
#endif

standalone long x[N];
standalone long y[N];
standalone long z[N];
standalone long w[N];
standalone long ci[N];

standalone long cx[K];
standalone long cy[K];
standalone long cz[K];
standalone long cw[K];

#define N_LONG_PAGE 512

#define N_LOCAL_CENTROIDS (K+N_LONG_PAGE)*N/PSIZE

// for part i, centroid kk:
// (K+N_LONG_PAGE)*i + kk
standalone long lcx[N_LOCAL_CENTROIDS];
standalone long lcy[N_LOCAL_CENTROIDS];
standalone long lcz[N_LOCAL_CENTROIDS];
standalone long lcw[N_LOCAL_CENTROIDS];
standalone long lcn[N_LOCAL_CENTROIDS];

// standalone long trigger[N];
// opt
// Note: each trigger's content should not > N_LONG_PAGE Bytes
standalone long trigger[(N/PSIZE+1)*N_LONG_PAGE];
// standalone long changed[N]; // obsolete. Not used anymore.

standalone long iter;

register long reg1;
register long reg2;
register long reg3;
register long reg4;

// register usage:
// distance_sq_2

#define distance_sq_2(x1, y1, z1, w1, x2, y2, z2, w2, d) \
reg1 = (x1) - (x2) ; \
reg2 = (y1) - (y2) ; \
reg3 = (z1) - (z2) ; \
reg4 = (w1) - (w2) ; \
*(d) = reg1*reg1 + reg2*reg2 + reg3*reg3 + reg4*reg4

#define distance_sq(x1, y1, z1, w1, x2, y2, z2, w2) \
(((x1) - (x2)) * ((x1) - (x2))) + \
(((y1) - (y2)) * ((y1) - (y2))) + \
(((z1) - (z2)) * ((z1) - (z2))) + \
(((w1) - (w2)) * ((w1) - (w2)))

void srand(long seed);

long rand();

void _output_centroids(long with_point_num);

void output_centroids_with_point_num();

void output_centroids();

void on_recluster_finished(long expected_iter);

void recluster_runner(long start, long end);

void kmeans_finish();

void start_iteration();

void on_init1_finished();

void init_runner(long start, long end);

void init_runner_stdin(long start, long end);

void init0();

#endif
