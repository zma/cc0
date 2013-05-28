long flag;
long flag2;
void void_watcher_commit()
{
    commit;
}

void void_runner_commit()
{
    flag = 1;   // Trigger the watcher
    
    commit;
}

void main()
{
    runner void_runner_commit();
    runner void_watcher_commit()
        watching flag
        using flag2
        ;
        

    commit;
}


