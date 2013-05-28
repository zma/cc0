long g_numbers[10];
long N;
long zzzzz_dummy;

void input()
{
    long i;
    g_numbers[0] = 0xfef3f6f4f3f2f1;
    
    i = 1;
input_j1:

    g_numbers[i] = 0xfef3f6f4f3f2f1 * g_numbers[i-1];

    i = i + 1;
    if (i < N) {
        goto input_j1;
    }

    return;
}

void output()
{
    long i;
    long tmp;

    i = 0;
output_j1:

    tmp = g_numbers[i];

    *(long *)(0x100000208) = tmp;

    i = i + 1;
    if (i < N) {
        goto output_j1;
    }

    return;
}

void main()
{
    long tmp;

    N = 10;

    tmp = 22222222;
    *(long *)(0x100000208) = tmp;

    output();


    input();

    tmp = 33333333;
    *(long *)(0x100000208) = tmp;

    output();
    
    return;
}
