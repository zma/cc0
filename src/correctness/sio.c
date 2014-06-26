#include <stdio.h>
#include <sio.h>

void runner1()
{
    sid_t sid;
    char buf[100];
    char *addr;
    uint64_t n;

    sid = slisten(8899);
    swrite(sid, &buf[0], 10);
    sread(sid, 10, &addr, &n);

    output_char('O');
    output_char('K');
    output_char(C_n);
    return;
}

void main()
{
    runner1();

    commit;
}


