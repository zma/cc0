#include "kmeans-common.h"

long rand()
{
    long r;

    r = *((long *)0x300000000);
    r = r * 0xfef3f6f4f3f2f1;
    *((long *)0x300000000) = r;
    return r;
}

void srand(long seed)
{
    if (seed == 0) {
        seed = 1;
    }

    *((long *)0x300000000) = seed;
    return;
}

