void main()
{
    long my_key;
    long div;
    long res;

    res = 1;
    my_key = 1024;
    div = 3;

    *(long *)(0x100000208) = res;
    res = my_key / div;
    *(long *)(0x100000208) = res;

    commit;
}
