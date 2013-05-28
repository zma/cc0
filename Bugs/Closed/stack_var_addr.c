/*
 * BUG: pointer casting
 * 
 * DESCRIPTION
 *      the generated DISA code is not correct
 * 
 * EXPECTED
 * 
 * ACTUAL
 *
 * The generated code:
 *
 * 400000193>  add:sq  $0xFFFFFFFFFFFFFFD0, $0x0,
 * 0xFFFFFFF0(0x200000000)
 * 4000001B2>  mov 0xFFFFFFF0(0x200000000):msq, 0x200000020:msq
 *
 * => in 0x200000020: $0xFFFFFFFFFFFFFFD0
 *
 * 4000001C9>  mov (0x200000020):msq, 0xFFFFFFE8(0x200000000):msq
 *
 * => use $0xFFFFFFFFFFFFFFD0 as address. 
 *
 * CHANGELOG
 *      May. 1, 2012          reported by Zhiqiang
 *      May. 1, 2012          fixed by Xiang. The bug is related to taking address of local (stack) variables.
 *      may. 1, 2012          confirmed by Zhiqiang. Closed.
 */



void main()
{
    double d1;
    double d2;
    double d3;
    long l;
    d1 = 3.14159;
    d2 = 2.0;

    d3 = d1 / d2;

    // seems not work

    l = *((long*)(&d3));

    // type cast
    // l = (long)d3;

    *(long *)(0x100000208) = l;
    
    commit;
}


