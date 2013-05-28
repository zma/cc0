long numbers[100];

void sort_runner_watcher()
{
    commit;
}

void main()
{
    //runner (numbers[0]) { sort_runner_watcher(); }
    runner sort_runner_watcher() 
        watching numbers[0,,100]
        using numbers[0,,100];

    commit;
}


