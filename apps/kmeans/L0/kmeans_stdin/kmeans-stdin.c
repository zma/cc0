#include "../kmeans_common/kmeans-common.h"


void init0_stdin()
{
    long i;

    for(i = 0; i < N; i = i + PSIZE)
    {
        trigger[i/PSIZE*N_LONG_PAGE] = 0;
    }

    runner init_runner_stdin(0, N) 
            using x[0,, N], y[0,, N], z[0,, N], w[0,, N], trigger[0/PSIZE*N_LONG_PAGE, , (N/PSIZE+1)*N_LONG_PAGE]
    ;

    runner on_init1_finished() 
        using cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K], x[0, , K], y[0, , K], z[0, , K], w[0, , K], /* changed[0, , N], */ trigger[0, , N], iter
        watching trigger[0, , (N/PSIZE+1)*N_LONG_PAGE];

    commit;
}


void main()
{
#ifdef DEBUG
    *((long *)STDOUT) = 'I';
    *((long *)STDOUT) = 10;
#endif

#ifdef INPUT_PMEM
    runner init0() using trigger[0, , (N/PSIZE+1)*N_LONG_PAGE], pmem_in, pmem_out;
#else // input from stdin
    // runner init0() using trigger[0, , (N/PSIZE+1)*N_LONG_PAGE];
    runner init0_stdin() using trigger[0, , (N/PSIZE+1)*N_LONG_PAGE];
#endif
    commit;
}

