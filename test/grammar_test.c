long dummy;
// sys configuration
standalone long N_LONG_PER_PAGE;

standalone long N_PAGES;

// number of numbers to sort
standalone long N;

// number of runners
standalone long N_RUNNER;

// numbers for each runner
standalone long N_PER_RUNNER;

// N_LONG_PER_PAGE= 512;
// N_PAGES = 10240;
// 
// N = N_LONG_PER_PAGE * N_PAGES;

// long g_numbers[N];
// long g_numbers[1024000*512+2*512];
standalone long g_numbers[2];
standalone long g_output_numbers[524298024];

// 512*N_RUNNER
standalone long g_runner_flag[1024];

standalone long g_watcher_flag0;

void output_q(long l)
{
    *(long *)(0x100000208) = l;

    return;
}

void output()
{
    long i;
    long tmp;

    i = 0;
output_j1:

    // output_q(g_numbers[i]);
    tmp = g_numbers[i];

    *(long *)(0x100000208) = tmp;

    i = i + 1;
    if (i < N) {
        goto output_j1;
    }

    return;
}

void output_result()
{
    long i;
    long tmp;

    long tmp2;

    // tmp2 = N + N_RUNNER * N_LONG_PER_PAGE;
    tmp2 = N;

    i = 0;
output_result_j1:

    // output_q(g_numbers[i]);
    tmp = g_output_numbers[i];

    *(long *)(0x100000208) = tmp;

    i = i + 1;
    if (i < tmp2) {
        goto output_result_j1;
    }

    return;
}

void input()
{
    long i;
    long tmp;

    tmp = N + N_RUNNER * N_LONG_PER_PAGE;

    // g_numbers[0] = (long)g_numbers;
    g_numbers[0] = 0xfef3f6f4f3f2f1;
    
    i = 1;
input_j1:

    g_numbers[i] = 0xfef3f6f4f3f2f1 * g_numbers[i-1];

    i = i + 1;
    if (i < tmp) {
        goto input_j1;
    }

    return;
}

void quick_sort(long start, long end)
{
    long pivot;
    long i;
    long j;
    long p;
    long tmp;

    // bug related comparison for signed integers
    // the comparision is actually done for unsigned integers
    // bug here: if (start >= end) {
    // if (end <= start) {
    tmp = start - end;
    if (tmp == 1) {
        goto quick_sort_j1;
    }
    if (tmp == 0) {
        goto quick_sort_j1;
    }
    if (tmp == 2) {
        goto quick_sort_j1;
    }

     
    // have bug:
    tmp = start;
    if (tmp >= end) {
        goto quick_sort_j1;
    }
    

    
    // have bug:
    if (start >= end) {
        goto quick_sort_j1;
    }
    

    pivot = g_numbers[end];

    i = start - 1;
    j = start;

quick_sort_j2:
    if (g_numbers[j] <= pivot) {
        // exchange j and i + 1
        i = i + 1;
        tmp = g_numbers[j];
        g_numbers[j] = g_numbers[i];
        g_numbers[i] = tmp;
    }

    j = j + 1;

    if (j <= end) {
        goto quick_sort_j2;
    }

    quick_sort(start, i - 1);
    quick_sort(i + 1, end);

quick_sort_j1:
    return;
}

void runner_quick_sort(long id)
{
    // start = id * N_PER_RUNNER
    // end = start + N_PER_RNNER - 1
    long start;
    long end;
    long tmp;

    tmp = N_PER_RUNNER + N_LONG_PER_PAGE;

    start = id * tmp;
    end = start + N_PER_RUNNER - 1;

    // output_q(start);
    // output_q(end);

    quick_sort(start, end);

    return;
}

void sort_runner(long id)
{
    long tmp;

    // output_q(999);

    runner_quick_sort(id);

    // output();

    g_runner_flag[512*id] = 1;

    // output_q(666);
    // output_q(g_runner_flag0);
    // output_q(g_runner_flag1);

    commit;
}

void merge_runner()
{
    long i;
    long j;
    
    long ni;
    long nj;

    long endi;
    long endj;

    long k;

    long tmp;

    // output_q(333);

    tmp = N_PER_RUNNER + N_LONG_PER_PAGE;

    i = 0 * tmp;
    endi = i + N_PER_RUNNER;

    j = 1 * tmp;
    endj = j + N_PER_RUNNER;

    // working here

    k = 0;

    ni = g_numbers[i];
    nj = g_numbers[j];

merge_runner_j1:

    if (ni <= nj) {
        g_output_numbers[k] = ni;
        i = i + 1;
        if (i < endi) {
            ni = g_numbers[i];
        }
        else {
            ni = -1;
        }
    }
    else {
        g_output_numbers[k] = nj;
        j = j + 1;
        if (j < endj) {
            nj = g_numbers[j];
        }
        else {
            nj = -1;
        }
    }

    k = k + 1;
    if (k < N) {
        goto merge_runner_j1;
    }

    // for debug only
    // output_result();

    commit;
}

void sort_runner_watcher()
{
    long runner_flags;

    runner_flags = g_runner_flag[0*512] + g_runner_flag[1*512];

    // output_q(555);
    if (runner_flags == 2) {
        if (g_watcher_flag0 == 1) {
            // the merge_runner is created already
            abortd;
        }
        else {
            // create merge runner now
            g_watcher_flag0 = 1;
            // output_q(444);
            runner merge_runner();
            commitd;
        }
    }
else {
    // abort but keep watching
    goto sort_runner_watcher_j1;
}
sort_runner_watcher_j1:
    abort;
}

void input_runner_watcher()
{
    long runner_flags;
    long tmp;

    tmp = g_runner_flag[0*512];

    // TODO: Debug here:
    // add this line will not compile
    tmp = g_runner_flag[1*512];

    commit;
}


void input_runner()
{
    // output_q(888);

    input();
   
    // output();

    runner sort_runner(0);
    runner sort_runner(1); 

    g_watcher_flag0 = 0;

    runner sort_runner_watcher() 
	watching g_runner_flag[0*512];
    runner sort_runner_watcher()
	watching g_runner_flag[1*512];

    commit;
}


void main()
{
    long i;
    long tmp;

    N_LONG_PER_PAGE= 512;
    N_PAGES = 1024000;

    N = N_LONG_PER_PAGE * N_PAGES;
    // for debug
    // N = 1024;

    N_RUNNER = 2;

    // debug here:
    // N_PER_RUNNER = N; //  / N_RUNNER;
    N_PER_RUNNER = 262144000; //  / N_RUNNER;
    // for debug
    // N_PER_RUNNER = 512;

    // output_q(777);

    // initialize flag
    g_runner_flag[0*512] = 0; // each runner update one page
    g_runner_flag[1*512] = 0; // each runner update one page

    // input data
    runner input_runner();

    commit;
}


