#include "libi0/stdio.h"
#include "libi0/stddef.h"
#include "libi0/unistd.h"

// watch an aborted task's exit code

standalone long status;

void t0()
{
    status = 1;
    commit;
}

void t1()
{
    long id;
    long ec;
    long ec_len;

    id = get_my_task_id();

    task_id_to_ec_range(id, &ec, &ec_len);

    *(long*)ec = 0x8;

    status = 1;

    abort;
}

void t2(long watched_ec)
{

    output_q(*(long*)watched_ec);
    output_char(C_n);

    output_char('O');
    output_char('K');
    output_char(C_n);

    commitd;
}

void main()
{
    long id;
    long ec;
    long ec_len;
    char *eca;

    // will abort
    runner t1()
        using status
        ;

    id = get_new_task_id();
    task_id_to_ec_range(id, &ec, &ec_len);

    eca = (char*)ec;

    runner t2(ec)
        using status
        watching eca[0,,ec_len]
        ;

    commit;
}

