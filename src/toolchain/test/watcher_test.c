long flag;

void runner_commit(long iterations)
{
    flag = flag + 1;   // Trigger the watcher
    
    commit;
}

void watcher_commit()
{
    if (flag == 1) {
        runner { runner_commit(); }
        commit;
    } else {
        commitd;
    }
}

void main()
{

    // initial value
    flag = 0;

    runner { runner_commit(); }

    runner (flag) { watcher_commit(); }

    commit;
}


