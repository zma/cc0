#pragma once

#include <stdint.h>

static int64_t inline __MIN(int64_t a, int64_t b)
{
    return (a < b ? a: b);
}

static int64_t inline __MAX(int64_t a, int64_t b)
{
    return (a > b ? a: b);
}



static int64_t inline __ROUND_UP(int64_t num, int64_t align)
{
    if (align == 0)
    {
        return num;
    }
    
    return (((num % align) == 0) ? (num) : (num + align - (num % align)));
}

static int64_t inline __ROUND_DOWN(int64_t num, int64_t align)
{
    if (align == 0)
    {
        return num;
    }
    
    return (num - (num % align));
}


#ifndef MIN
#define MIN(a,b) __MIN(a,b)
#endif

#ifndef MAX
#define MAX(a,b) __MAX(a,b)
#endif

#ifndef ROUND_DOWN
#define ROUND_DOWN(x,a) __ROUND_DOWN(x,a)
#endif


#ifndef ROUND_UP
#define ROUND_UP(x,a) __ROUND_UP(x,a)
#endif