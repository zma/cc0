#include "kmeans-common.h"

void _output_centroids(long with_point_num)
{
    long i;
    long *npt;
    npt = (long *)(0x300000000 + K * 8 * 4);
    output_q(K);
    output_char(' ');
    output_char('c');
    output_char('e');
    output_char('n');
    output_char('t');
    output_char('r');
    output_char('o');
    output_char('i');
    output_char('d');
    output_char('s');
    output_char(':');
    output_char(C_n);
    for(i = 0; i < K; i = i + 1)
    {
        output_q(cx[i]);
        output_char(' ');
        output_q(cy[i]);
        output_char(' ');
        output_q(cz[i]);
        output_char(' ');
        output_q(cw[i]);
        if (with_point_num == 1) {
            output_char('/');
            output_q(npt[i]);
        }
        output_char(C_n);
    }
    output_char(C_n);

    return;
}
