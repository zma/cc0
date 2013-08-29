#include "libi0/stdio.h"

register long R1;
register long R2;
register long R3;
register long R4;

long data[1000];

void runner1()
{
    output_char('O');
    output_char('K');
    output_char(C_n);
    commit;
}

void main()
{
    long n;

    // asm("newr 0x8, 0x8, 0x8, 0x8");
    // asm("newr R1, R2, R3, R4");
    // asm("exit:c");

    // asm("nop");

    runner runner1()
        using data[0,,800];

    n = 0x44;

    // asm("nop");

    // runner runner1()
    //     using data[0,,n];

    commit;
}


