standalone long mark[2048];

void runner_1()
{
    long i,tmp;
    i = 0;
    tmp = i * (1023+1);
    mark[tmp] = 1;
    *(long *)(0x100000208) = 1111;
    commit;
}

void runner_2()
{
    long i,tmp;
    i = 1;
    tmp = i * (1023+1);
    mark[tmp] = 2;
    *(long *)(0x100000208) = 2222;
    commit;
}

void watcher()
{
    long i,tmp;
    *(long *)(0x100000208) = 3331;
    i = 0;
watcher_j1:
    if(i < 2)
    {
        tmp = i * 1024;
        if(mark[tmp] != (i+1))
        {
            goto watcher_j1_out;
        }
        i = i + 1;
        goto watcher_j1;	
    }
    *(long *)(0x100000208) = 3333;    
    commitd;
    
watcher_j1_out:
    *(long *)(0x100000208) = 3334;
    abort;	
}

void main()
{
    mark[0] = -1;
    mark[1024] = -1;
    	
    runner runner_1();    
        
    runner runner_2() using mark[0,,1024] watching mark[0];
    
    runner watcher() using mark[0,,1024] watching mark[0,,1024];
    

    commit;
}