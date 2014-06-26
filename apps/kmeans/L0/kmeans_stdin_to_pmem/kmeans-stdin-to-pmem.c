#include "../kmeans_common/kmeans-common.h"

void shuffle_runner(long start, long end)
{
    long i, j, mini;
    long mind, d;
    long xt, yt, zt, wt;
    long partid;

#ifdef DEBUG
    *((long *)STDOUT) = 'R';
#endif

    partid = start / PSIZE;

    for(i = start; i < end; i = i  + 1)
    {
        xt = x[i];
        yt = y[i];
        zt = z[i];
        wt = w[i];
        pmem_in[4*i]   = xt;
        pmem_in[4*i+1] = yt;
        pmem_in[4*i+2] = zt;
        pmem_in[4*i+3] = wt;
    }
    trigger[start/PSIZE*N_LONG_PAGE] = 2;
    commitd;
}

void start_shuffle()
{
    long i;
    long tmp1;
    long tmp2;

#ifdef DEBUG
    *((long *)STDOUT) = 'S';
#endif
    for(i = 0; i < N; i = i + PSIZE)
    {
        trigger[i/PSIZE*N_LONG_PAGE] = 0;

#ifdef LOCAL_AGGREG
        tmp1 = (K+N_LONG_PAGE)*i/PSIZE;
        tmp2 = tmp1 + K + N_LONG_PAGE;
#endif

        runner 
            shuffle_runner(i, i + PSIZE) 
            using x[i, , i + PSIZE], y[i, , i + PSIZE], z[i, , i + PSIZE], w[i, , i + PSIZE], ci[i, , i + PSIZE], cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K], trigger[i/PSIZE*N_LONG_PAGE, , (i/PSIZE+1)*N_LONG_PAGE]
        , pmem_in 
        , pmem_in[4*i,, 4*(i + PSIZE)]
	, lcx[tmp1,, tmp2], lcy[tmp1,, tmp2], lcz[tmp1,, tmp2], lcw[tmp1,, tmp2], lcn[tmp1,, tmp2]
	;
    }
    return;
}


void on_init1_finished_stdin_only()
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

    // shuffle the input
    start_shuffle();

    commitd;
}


void init0_stdin_only()
{
    long i;

    pmem_in = (long*)PMEM_IN_ADDR;

    for(i = 0; i < N; i = i + PSIZE)
    {
        trigger[i/PSIZE*N_LONG_PAGE] = 0;
    }

    runner init_runner_stdin(0, N) 
            using x[0,, N], y[0,, N], z[0,, N], w[0,, N], trigger[0/PSIZE*N_LONG_PAGE, , (N/PSIZE+1)*N_LONG_PAGE]
            , pmem_in
            // , pmem_in[0,,PMEM_IN_SIZE]
    ;

    runner on_init1_finished_stdin_only() 
        using cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K], x[0, , K], y[0, , K], z[0, , K], w[0, , K], /* changed[0, , N], */ trigger[0, , N], iter
        , pmem_in
        watching trigger[0, , (N/PSIZE+1)*N_LONG_PAGE]
        ;

    commit;
}


void main()
{
#ifdef DEBUG
    *((long *)STDOUT) = 'I';
    *((long *)STDOUT) = 10;
#endif

    runner init0_stdin_only() using trigger[0, , (N/PSIZE+1)*N_LONG_PAGE],
           pmem_in;
    commit;
}

