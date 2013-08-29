long flag;

void void_runner_commit(long i)
{
    flag = i;
    
    *(long *)(0x100000208) = i;
    commit;
}

void main()
{
    runner void_runner_commit(0xcd) using flag;

    commit;
}


