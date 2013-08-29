#include "libi0/stdio.h"

char *pmem_in;

void work0()
{
    output_char('v');
    output_char('A');
    output_q_hex('A');
    output_char(C_n);
    return;
}

void work1()
{
    char t;
    output_char('w');
    output_char(*(char*)0x0000088000000002);
    output_q_hex(*(char*)0x0000088000000002);
    output_char(C_n);
    t = pmem_in[2];

    // output_char(pmem_in[2]);
    // output_q_hex(pmem_in[2]);
    output_char(C_n);
    output_char(*(char*)0x0000088000000002);
    output_q_hex(*(char*)0x0000088000000002);
    output_char(C_n);
    return;
}

void work2()
{
    output_char('x');
    output_char(*(char*)0x0000088000000002);
    output_q_hex(*(char*)0x0000088000000002);
    output_char(C_n);
    output_char(*(pmem_in+2));
    output_q_hex(*(pmem_in+2));
    output_q_hex(*(char*)0x0000088000000002);
    output_char(C_n);
    return;
}


void work_()
{
    long t;

    pmem_in[2] = 'A';
    t = *(long*)0x200000018;
    output_char('t');
    output_q_hex(t);
    output_char(C_n);
    output_char(*(char*)0x0000088000000002);
    output_q_hex(*(char*)0x0000088000000002);
    output_char(C_n);
    *(pmem_in+2) = 'A';
    output_char('u');
    output_char(*(char*)0x0000088000000002);
    output_q_hex(*(char*)0x0000088000000002);
    output_char(C_n);
    work0();
    work1();
    work2();
    return;
}

void work()
{
    long t;

    pmem_in[2] = 'A';
    output_char(pmem_in[2]);
    output_char(C_n);
    return;
}


void init()
{
    pmem_in = (char*)(0x88000000000);

    runner work()
        using pmem_in, pmem_in[0,,100];
    return;
}

void main()
{
    runner init()
        using pmem_in;
    commit;
}


