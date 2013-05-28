
// void void_runner_commit(long i)
void void_runner_commit()
{
    long i;
    long j;
    i = 0;
    j = 0;
loop1:
    j = j + 1;
    i = i + 1;
    if (i < 1000) {
        goto loop1;
    }
    commit;
}

void main()
{
    long i;
    i = 0;
loop2:
    // runner { void_runner_commit(); }
    i = i + 1;
    if (i < 100) {
        goto loop2;
    }
    commit;
}


