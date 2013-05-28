
// sys configuration
long N_LONG_PER_PAGE;

long N_PAGES;

// number of numbers to sort
long N;

// number of runners
long N_RUNNER;

// numbers for each runner
long N_PER_RUNNER;

// N_LONG_PER_PAGE= 512;
// N_PAGES = 10240;
// 
// N = N_LONG_PER_PAGE * N_PAGES;

// long g_numbers[N];
// long g_numbers[10240*512];
long g_numbers[5242880];

void input()
{
    long i;
    long tmp;
    long tmp2;

    g_numbers[0] = 100298734878743;
    i = 1;

input_j1:
    tmp2 = i - 1;
    tmp = g_numbers[tmp2];
    g_numbers[i] = 100298734878743 * tmp;

    i = i + 1;
    if (i < N) {
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

    start = id * N_PER_RUNNER;
    end = start + N_PER_RUNNER - 1;

    quick_sort(start, end);

    return;
}

void sort_runner(long id)
{
    // quick_sort(id);
    runner_quick_sort(0);

    commit;
}

void main()
{
    long i;

    N_LONG_PER_PAGE= 512;
    N_PAGES = 10240;

    N = N_LONG_PER_PAGE * N_PAGES;

    N_RUNNER = 1;

    N_PER_RUNNER = N / N_RUNNER;

    // input data
    runner { input(); }

    i = 0;
    runner { sort_runner(i); }

    commit;
}


