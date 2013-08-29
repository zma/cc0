double d1, d2;
standalone double d3, d4;

void add_runner_commit1()
{
    d3 = d1 + d2;
    commit;
}

void add_runner_commit2()
{
    d4 = d1 + d1;
    commit;
}

void add_runner_abort()
{
    d4 = d1 - d2;
    d3 = d4;
    abort;
}
void main()
{
    d1 = 3.14159;
    d2 = 2.0;

    d3 = 1.23456;
    d4 = 1.23456;

    runner { add_runner_commit1(); }
    runner { add_runner_commit2(); }
    runner { add_runner_abort(); }
    
    // After both sub-runners commit, d3 should be 5.14159, d4 should be 6.28318
    
    commit;
}


