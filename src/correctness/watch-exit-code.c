#include "stdio.h"
#include "stddef.h"

// watch an to-commit-successfully task's exit code

standalone long status;

void t1()
{
    long id;
    long ec;
    long ec_len;

    id = get_my_task_id();

    task_id_to_ec_range(id, &ec, &ec_len);

    *(long*)ec = 0x8;

    status = 1;

    commit;
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


    runner t1()
        using status
        ;

    id = get_new_task_id();
    task_id_to_ec_range(id, &ec, &ec_len);

    runner t2(ec)
        using status
        watching ((char*)ec)[0,,ec_len]
        ;

    commit;
}

