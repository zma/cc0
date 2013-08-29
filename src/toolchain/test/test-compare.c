long a;
long b;

void output_q(long l)
{
    *(long *)(0x100000208) = l;

    return;
}

void runner1()
{
    if (a <= b) {
        output_q(111);
    }
    else {
        output_q(222);
    }

    commit;
}

void main()
{
    a = -800;
    // a = 800;
    b = 100;

    runner runner1() using a,b;
    commit;
}

