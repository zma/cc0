#define _DEBUG_MLC_

#include "libi0/malloc.h"

void null_runner()
{
    commit;
}
void main()
{
    long *a;
    
    a = (long *)shalloc(0x100);
    runner null_runner();
    a = (long *)shalloc(0x100);
    
    commit;
}
