void stdin_stdout_runner()
{
    long l;

    l = 0;

    l = *(long *)(0x100000200);
    //l = *(long *)(4294967808);

    *(long *)(0x100000208) = l;
    //*(long *)(4294967816) = l;

    commit;
}

void main()
{
    runner stdin_stdout_runner();

    commit;
}


