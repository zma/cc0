#include <stdio.h>

standalone long status;

void t1()
{
    status = 1;
    abort;
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
    runner t1()
        using status;
    runner t2()
        using status
        watching status;

    commit;
}

