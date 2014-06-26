#include "kmeans-common.h"

void on_recluster_finished(long expected_iter)
{
    long i;
    long j;
    long *tlcx;
    long *tlcy;
    long *tlcz;
    long *tlcw;
    long *tlcn;

    long n_parts;

    long *sumx;
    long *sumy;
    long *sumz;
    long *sumw;
    long *npt;

#ifdef DEBUG
    *((long *)STDOUT) = 'E';
#endif

    if (expected_iter < iter) {
        // already moved on... I am too lost behind
        abortd;
    }

    if (expected_iter > iter) {
        // not reach there where I should start work yet.
        // abort and wait
        abort;
    }

    for(i = 0; i < N/PSIZE * N_LONG_PAGE; i = i + N_LONG_PAGE)
    {
        if(trigger[i] != 2)
        {
            abort;
        }
    }

    // mark this watcher committed
    // trigger[0] = 3;

    sumx = (long *)(0x300000000);
    sumy = (long *)(0x300000000 + K * 8);
    sumz = (long *)(0x300000000 + K * 8 * 2);
    sumw = (long *)(0x300000000 + K * 8 * 3);
    npt = (long *)(0x300000000 + K * 8 * 4);

    for(i = 0; i < K; i = i + 1)
    {
        sumx[i] = 0;
        sumy[i] = 0;
        sumz[i] = 0;
        sumw[i] = 0;
        npt[i] = 0;
    }

    // get sum and np
    n_parts = N / PSIZE;
    for (j = 0; j < n_parts; j = j + 1) {
        tlcx = &lcx[(K+N_LONG_PAGE)*j];
        tlcy = &lcy[(K+N_LONG_PAGE)*j];
        tlcz = &lcz[(K+N_LONG_PAGE)*j];
        tlcw = &lcw[(K+N_LONG_PAGE)*j];
        tlcn = &lcn[(K+N_LONG_PAGE)*j];
	for (i = 0; i < K; i = i + 1)
	{
	    sumx[i] = sumx[i] + tlcx[i];
	    sumy[i] = sumy[i] + tlcy[i];
	    sumz[i] = sumz[i] + tlcz[i];
	    sumw[i] = sumw[i] + tlcw[i];
	    npt[i] = npt[i] + tlcn[i];
        }
    }

    for(i = 0; i < K; i = i + 1)
    {
        if (npt[i] != 0) {
            cx[i] = sumx[i] / npt[i];
            cy[i] = sumy[i] / npt[i];
            cz[i] = sumz[i] / npt[i];
            cw[i] = sumw[i] / npt[i];
        }
        else {
            // disable it
            cx[i] = 0x7FFFFFFFFFFFFFF;
            cy[i] = 0x7FFFFFFFFFFFFFF;
            cz[i] = 0x7FFFFFFFFFFFFFF;
            cw[i] = 0x7FFFFFFFFFFFFFF;
        }
    }

#ifdef PRINT_ITER
    output_char('i');
    output_char('t');
    output_char('e');
    output_char('r');
    output_char(':');
    output_q(iter);
    output_char(C_n);
#endif

#ifdef DEBUG
    output_centroids_with_point_num();
#endif

    // iter is a guard to avoid multiple tasks for the same
    // expected_iter commit successfully

    // Note: must be done before start_iteration
    iter = iter + 1;

    if (iter < MAX_ITER) {
        start_iteration();
    }
    else {
        kmeans_finish();
    }

#ifdef DEBUG
    *((long *)STDOUT) = 'T';
    output_char(C_n);
    output_q(iter);
    output_char(C_n);
#endif

    commitd;

}
