#include "../kmeans_common/kmeans-common.h"

void init_runner_pmem(long start, long end)
{
    long i;
    long t;

#ifdef DEBUG
    *((long *)STDOUT) = 'I';
#endif

    for(i = start; i < end; i = i + 1)
    {
        x[i] = pmem_in[4*i];
#ifdef DEBUG_MORE
        output_q(x[i]);
        output_char(' ');
#endif
        y[i] = pmem_in[4*i+1];
#ifdef DEBUG_MORE
        output_q(y[i]);
        output_char(' ');
#endif
        z[i] = pmem_in[4*i+2];
#ifdef DEBUG_MORE
        output_q(z[i]);
        output_char(' ');
#endif
        w[i] = pmem_in[4*i+3];
#ifdef DEBUG_MORE
        output_q(w[i]);
        output_char(C_n);
#endif
    }

    trigger[start/PSIZE*N_LONG_PAGE] = 1;

    commit;
}


void init0_in_pmem()
{
    long i;

    pmem_in = (long*)PMEM_IN_ADDR;

    for(i = 0; i < N; i = i + PSIZE)
    {
        trigger[i/PSIZE*N_LONG_PAGE] = 0;
        runner init_runner_pmem(i, i + PSIZE) 
            using x[i, , i + PSIZE], y[i, , i + PSIZE], z[i, , i + PSIZE], w[i, , i + PSIZE], trigger[i/PSIZE*N_LONG_PAGE, , (i/PSIZE+1)*N_LONG_PAGE]
            , pmem_in
            , pmem_in[4*i,, 4*(i + PSIZE)]
            ;
    }

    runner on_init1_finished() 
        using cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K], x[0, , K], y[0, , K], z[0, , K], w[0, , K], /* changed[0, , N], */ trigger[0, , N], iter
        , pmem_in
        watching trigger[0, , (N/PSIZE+1)*N_LONG_PAGE];

    commit;
}



void main()
{
#ifdef DEBUG
    *((long *)STDOUT) = 'M';
    *((long *)STDOUT) = 10;
#endif

    runner init0_in_pmem() using trigger[0, , (N/PSIZE+1)*N_LONG_PAGE],
           pmem_in;

    commit;
}

