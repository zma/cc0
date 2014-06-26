#include "stdio.h"
#include "stddef.h"

standalone long status;

void t1()
{
    long id;
    long ec;
    long ec_len;

    id = get_my_task_id();

    task_id_to_ec_range(id, &ec, &ec_len);

    *(long*)ec = 0x8;

    output_q(*(long*)ec);
    output_char(C_n);

    output_char('O');
    output_char('K');
    output_char(C_n);

    commitd;
}

void main()
{
    runner t1()
        using status
        ;

    commit;
}
