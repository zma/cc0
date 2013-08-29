long flag;
// void void_runner_commit(long i)
void void_watcher_commit()
{
    commit;
}

// void void_runner_commit(long i)
void void_runner_commit(long iterations)
{
    flag = 1;   // Trigger the watcher
    
    commit;
}

void main()
{
    runner { void_runner_commit(10000); }
    runner (flag) { void_watcher_commit(); }

    commit;
}


