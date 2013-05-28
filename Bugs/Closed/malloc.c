/*
 * BUG: fun call fails to return
 * 
 * DESCRIPTION
 *   fun call fails to return
 * 
 * EXPECTED
 *   return to caller function
 * 
 * ACTUAL
 *  10 4000000C1>  mov 0x200000000:sq, $0xFFFFFFF8(0x200000008):sq
 *  11 4000000D8>  sub:sq  0x200000008, $0x8, 0x200000000
 *  12 4000000F3>  sub:sq  0x200000008, $0x8, 0x200000008
 *  13 40000010E>  mov 0x200000000:sq, 0x200000008:sq
 *  14 400000121>  mov (0x200000008):sq, 0x200000000:sq
 *  15 400000134>  sub:sq  0x200000008, $0x8, 0x200000008
 *  16 40000014F>  br:ji   (0x200000008)
 *
 *  21 4000001C1>  mov $0x0:sq, $0x8(0x200000008):sq
 *  22 4000001D8>  br:j a  0x4000000C1
 *
 * the return address is fill with 0x0. (in line 21).
 * 
 * CHANGELOG
 *      April 21, 2012          reported by Zhiqiang
 *      April 22, 2012          fixed by Xiang.
 *      April 24, 2012          verified by Zhiqiang
 */

long _pMallocSR;
long *data;
void *malloc(long size)
{
    void *p;
    p = (void *)_pMallocSR;
    _pMallocSR = _pMallocSR + size;;
    return p;
}

void run()
{
    long i;
    for (i = 0; i < 1000; i = i + 1)
    {
        data[i] = i;
        *(long *)(0x100000208) = i;
    }    
    
    commit;
}

void init0()
{
    _pMallocSR = 0x500000000;
    data = (long *)malloc(1000);
    
    runner run() using data[0,,1000];
    
    commit;
}

void main()
{
    runner init0() using _pMallocSR, data;

    commit;
}
