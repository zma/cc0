/*
 * BUG: stack variable address
 * 
 * DESCRIPTION
 *      br jumpts to a target by label
 * 
 * EXPECTED
 *      fills the target for a label as the function name
 * ACTUAL
 *      fails to compile
 *
 * CHANGELOG
 *      Jun. 1, 2012          reported by Zhiqiang
 *      Jun. 4, 2012          verified by Zhiqiang
 */

void main()
{
    long n;

myloop:
    asm("br:le:sq a         0xFFFFFFE8(0x200000000), $0x0, $0x40000019B");
    asm("br:le:sq a         0xFFFFFFE8(0x200000000), $0x0, $main");
    // error at this line
    asm("br:le:sq a         0xFFFFFFE8(0x200000000), $0x0, $myloop");
    goto myloop;

    commit;
}

