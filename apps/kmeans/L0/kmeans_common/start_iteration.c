#include "kmeans-common.h"

void start_iteration()
{
    long i;
    long tmp1;
    long tmp2;

#ifdef DEBUG
    *((long *)STDOUT) = 'S';
#endif
    for(i = 0; i < N; i = i + PSIZE)
    {
        // changed[i] = 0;
        trigger[i/PSIZE*N_LONG_PAGE] = 0;

#ifdef LOCAL_AGGREG
        tmp1 = (K+N_LONG_PAGE)*i/PSIZE;
        tmp2 = tmp1 + K + N_LONG_PAGE;
#endif

        runner
#ifdef LOCAL_AGGREG
            recluster_runner(i, i + PSIZE)
#else
            recluster_runner_0(i, i + PSIZE)
#endif
            using x[i, , i + PSIZE], y[i, , i + PSIZE], z[i, , i + PSIZE], w[i, , i + PSIZE], ci[i, , i + PSIZE], cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K], trigger[i/PSIZE*N_LONG_PAGE, , (i/PSIZE+1)*N_LONG_PAGE]
            /* changed[i, , i + PSIZE], */
#ifdef OUTPUT_PMEM
        , pmem_in,  pmem_out
        , pmem_in[4*i,, 4*(i + PSIZE)]
#endif
#ifdef LOCAL_AGGREG
	// , lcx[0,, N_LOCAL_CENTROIDS], lcy[0,, N_LOCAL_CENTROIDS], lcz[0,, N_LOCAL_CENTROIDS], lcw[0,, N_LOCAL_CENTROIDS], lcn[0,, N_LOCAL_CENTROIDS]
	, lcx[tmp1,, tmp2], lcy[tmp1,, tmp2], lcz[tmp1,, tmp2], lcw[tmp1,, tmp2], lcn[tmp1,, tmp2]
#endif
	;
    }
    runner
#ifdef LOCAL_AGGREG
        on_recluster_finished(iter)
#else
        on_recluster_finished_0()
#endif
        using iter, cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K]
#ifdef OUTPUT_PMEM
        , pmem_in,  pmem_out
#endif
#ifdef OUTPUT_PMEM
        , pmem_in[0,,PMEM_IN_SIZE]
        , pmem_out[0,,PMEM_OUT_SIZE]
#endif

#ifdef LOCAL_AGGREG
        , lcx[0,, N_LOCAL_CENTROIDS], lcy[0,, N_LOCAL_CENTROIDS], lcz[0,, N_LOCAL_CENTROIDS], lcw[0,, N_LOCAL_CENTROIDS], lcn[0,, N_LOCAL_CENTROIDS]
#else
        , x[0, , N], y[0, , N], z[0, , N], w[0, , N], ci[0, , N]
#endif
        watching trigger[0, , (N/PSIZE+1)*N_LONG_PAGE]
        ;
    return;
}
