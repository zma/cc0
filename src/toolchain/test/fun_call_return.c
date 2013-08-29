void fun_call2()
{
    return;
}

void output_q(long n)
{
    *(long*)0x100000208 = n;
    return;
}


void output_char(char c)
{
    *(long *)(0x100000208) = c;
    return;
}

long fun_call(long l, long m)
{
    output_char('c');
    // *(long *)(0x100000208) = l;

    // *(long *)(0x100000208) = 100;
    output_q(l);
    output_q(m);

    // return l;
    return 0x999888;
}

void main()
{
    long tmp;
    output_char('a');
    tmp = fun_call(0x1000, 0x20000);
    output_char('b');
    output_q(tmp);

    commit;
}


