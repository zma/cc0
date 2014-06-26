#include "stdio.h"
#include "stddef.h"

standalone long status;

void t1()
{
    status = 1;
    commit;
}

void t2()
{
    output_char('O');
    output_char('K');
    output_char(C_n);

    commitd;
}

void main()
{
    long space;

    space = L0_SPACE0;

    runner t1()
        using status
        in space
        ;

    space = L0_SPACE1;
    runner t2()
        using status
        watching status
        in space
        ;

    commit;
}

