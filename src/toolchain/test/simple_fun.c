// void void_runner_commit(long i)
void void_runner_commit(long iterations)
{
    long i;
    long j;
   
    i = 0;
    j = 0;

loop1:
    j = j + 1;
    i = i + 1;
    
    if (i < iterations) {
        goto loop1;
    }
    
    return;
}

void main()
{
    void_runner_commit(10000);

    commit;
}


