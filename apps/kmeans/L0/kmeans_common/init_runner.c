#include "kmeans-common.h"

void init_runner(long start, long end)
{
    long i;
    long t;

// #ifdef INPUT_PMEM
// /*
//     for (i = start; i < end; i = i + 1)
//     {
//         x[i] = pmem_in[4*i];
//         y[i] = pmem_in[4*i+1];
//         z[i] = pmem_in[4*i+2];
//         w[i] = pmem_in[4*i+3];
// #ifdef DEBUG_PRINT_IN
//         output_char('P');
//         output_q(i);
//         output_char('@');
//         output_q_hex((long)&pmem_in[4*i]);
//         output_char(':');
//         output_q(x[i]);
//         output_char('#');
//         output_q(pmem_in[4*i]);
//         output_char(' ');
//         output_q(y[i]);
//         output_char(' ');
//         output_q(z[i]);
//         output_char(' ');
//         output_q(w[i]);
//         output_char(C_n);
// #endif
//     }
// */
// #else
    // default: generate randomly
    // srand(*(long*)(0x200000008));
    srand(1);

#ifdef INPUT_STDIN
    for(i = 0; i < start; i = i + 1)
    {
	    t = input_q();
	    t = input_q();
	    t = input_q();
	    t = input_q();
    }
#endif

    for(i = start; i < end; i = i + 1)
    {
#ifdef INPUT_STDIN
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
#else // INPUT_STDIN
        x[i] = (long)rand() / (long)0x100000000;
        y[i] = (long)rand() / (long)0x100000000;
        z[i] = (long)rand() / (long)0x100000000;
        w[i] = (long)rand() / (long)0x100000000;
#endif // INPUT_STDIN
    }
// #endif
    trigger[start/PSIZE*N_LONG_PAGE] = 1;
    commit;
}
