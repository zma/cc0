#include "kmeans-common.h"

void recluster_runner(long start, long end)
{
    long i, j, mini;
    long mind, d;
    long xt, yt, zt, wt;
    long partid;

    long *mylcx;
    long *mylcy;
    long *mylcz;
    long *mylcw;
    long *mylcn;


#ifdef DEBUG
    *((long *)STDOUT) = 'R';
#endif
    partid = start / PSIZE;

    mylcx = &lcx[(K+N_LONG_PAGE)*partid];
    mylcy = &lcy[(K+N_LONG_PAGE)*partid];
    mylcz = &lcz[(K+N_LONG_PAGE)*partid];
    mylcw = &lcw[(K+N_LONG_PAGE)*partid];
    mylcn = &lcn[(K+N_LONG_PAGE)*partid];

    for (i = 0; i < K; i = i + 1) {
        mylcx[i] = 0;
        mylcy[i] = 0;
        mylcz[i] = 0;
        mylcw[i] = 0;
        mylcn[i] = 0;
    }

    for(i = start; i < end; i = i  + 1)
    {
#ifdef INPUT_PMEM
        xt = pmem_in[4*i];
        yt = pmem_in[4*i+1];
        zt = pmem_in[4*i+2];
        wt = pmem_in[4*i+3];
#else
        xt = x[i];
        yt = y[i];
        zt = z[i];
        wt = w[i];
#endif
        j = 0;
        // d = distance_sq(xt, yt, zt, wt, cx[j], cy[j], cz[j], cw[j]);
        distance_sq_2(xt, yt, zt, wt, cx[j], cy[j], cz[j], cw[j], &d);

        mind = d;
        mini = j;

        for(j = 1; j < K; j = j + 1)
        {
            // d = distance_sq(xt, yt, zt, wt, cx[j], cy[j], cz[j], cw[j]);
            distance_sq_2(xt, yt, zt, wt, cx[j], cy[j], cz[j], cw[j], &d);

            if(d < mind)
            {
                mind = d;
                mini = j;
            }
        }
        mylcn[mini] = mylcn[mini] + 1;
        mylcx[mini] = mylcx[mini] + xt;
        mylcy[mini] = mylcy[mini] + yt;
        mylcz[mini] = mylcz[mini] + zt;
        mylcw[mini] = mylcw[mini] + wt;

        // if(ci[i] != mini)
        // {
        //     ci[i] = mini;
        //     // changed[start] = 1;
        // }
    }
    trigger[start/PSIZE*N_LONG_PAGE] = 2;
    commitd;
}
