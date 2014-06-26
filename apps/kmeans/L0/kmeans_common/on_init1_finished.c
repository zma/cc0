#include "kmeans-common.h"

void on_init1_finished()
{
    long i;
    for(i = 0; i < N/PSIZE * N_LONG_PAGE; i = i + N_LONG_PAGE)
    {
        if(trigger[i] != 1)
        {
            abort;
        }
    }

    // mark this watcher committed
    trigger[0] = 3;

    for(i = 0; i < K; i = i + 1)
    {
#ifdef INPUT_PMEM
        cx[i] = pmem_in[4*i];
        cy[i] = pmem_in[4*i+1];
        cz[i] = pmem_in[4*i+2];
        cw[i] = pmem_in[4*i+3];
#else
        cx[i] = x[i];
        cy[i] = y[i];
        cz[i] = z[i];
        cw[i] = w[i];
#endif
    }

    // number of iterations
    iter = 0;

#ifdef DEBUG
    output_char('i');
    output_char('t');
    output_char('e');
    output_char('r');
    output_char(':');

    output_q(-1);
    output_char(':');
    output_centroids();
#endif

    start_iteration();

    commitd;
}
