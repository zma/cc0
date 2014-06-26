#include "kmeans-common.h"

void init_runner_stdin(long start, long end)
{
    long i;
    long t;

    for(i = 0; i < start; i = i + 1)
    {
	    t = input_q();
	    t = input_q();
	    t = input_q();
	    t = input_q();
    }

    for(i = start; i < end; i = i + 1)
    {
        x[i] = input_q();
#ifdef DEBUG
        output_q(x[i]);
        output_char(' ');
#endif
        y[i] = input_q();
#ifdef DEBUG
        output_q(y[i]);
        output_char(' ');
#endif
        z[i] = input_q();
#ifdef DEBUG
        output_q(z[i]);
        output_char(' ');
#endif
        w[i] = input_q();
#ifdef DEBUG
        output_q(w[i]);
        output_char(C_n);
#endif
    }

    for(i = start; i < end; i = i + 1) {
        trigger[i/PSIZE*N_LONG_PAGE] = 1;
    }

    commit;
}
