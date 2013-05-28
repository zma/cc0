struct {
    long flag0;
    long flag1;
} flags;

void sort_runner(long id)
{
    /*
    // can not compile here
    if (id == 0) {
        flags.flag0 = 1;
    }
    else {
        flags.flag1 = 1;
    }
    */

    // can not compile here
    flags.flag0 = 1;

    commit;
}

void merge_runner()
{

    commit;
}

void sort_runner_watcher()
{
    commit;
}

void input_runner()
{
    runner sort_runner(0);
    runner sort_runner(1);

    runner sort_runner_watcher() 
        watching flags;
        
    runner sort_runner_watcher() 
        watching flags;
    
    commit;
}

void main()
{
    long i;
    long tmp;

    // input data
    runner input_runner();

    commit;
}


