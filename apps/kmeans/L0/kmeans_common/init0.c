#include "kmeans-common.h"

void init0()
{
    long i;
#ifdef INPUT_PMEM
    pmem_in = (long*)PMEM_IN_ADDR;
    pmem_out = (long*)PMEM_OUT_ADDR;
#endif
    for(i = 0; i < N; i = i + PSIZE)
    {
        trigger[i/PSIZE*N_LONG_PAGE] = 0;
        runner init_runner(i, i + PSIZE)
            using x[i, , i + PSIZE], y[i, , i + PSIZE], z[i, , i + PSIZE], w[i, , i + PSIZE], trigger[i/PSIZE*N_LONG_PAGE, , (i/PSIZE+1)*N_LONG_PAGE]
#ifdef INPUT_PMEM
            , pmem_in
#endif
	;
    }

    runner on_init1_finished()
        using cx[0, , K], cy[0, , K], cz[0, , K], cw[0, , K], x[0, , K], y[0, , K], z[0, , K], w[0, , K], /* changed[0, , N], */ trigger[0, , N], iter
#ifdef INPUT_PMEM
        , pmem_in, pmem_out
        , pmem_in[0,,PMEM_IN_SIZE]
        , pmem_out[0,,PMEM_OUT_SIZE]
#endif
        watching trigger[0, , (N/PSIZE+1)*N_LONG_PAGE];

    commit;
}
