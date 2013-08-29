#include "libi0/stdio.h"

void runner1()
{
    long i;
    for (i = 0x0; i < 0xFF; i = i + 1) {
        output_q(i);
        output_char(':');
        output_q_hex(i);
        output_char(C_n);
    }
    return;
}

void main()
{
    runner1();

    commit;
}


