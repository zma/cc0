#include "kmeans-common.h"

void kmeans_finish()
{
#ifdef OUTPUT_PMEM
    long i;

    for(i = 0; i < K; i = i + 1)
    {
        pmem_out[4*i] = cx[i];
        pmem_out[4*i+1] = cy[i];
        pmem_out[4*i+2] = cz[i];
        pmem_out[4*i+3] = cw[i];
    }
#endif

#ifdef OUTPUT_ON_FINISH
    output_centroids();
#endif
    return;
}
