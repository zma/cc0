/*
 * BUG: stack variable address
 * 
 * DESCRIPTION
 *      the generated DISA code is not correct
 * 
 * EXPECTED
 *      a: framepointer - 0x8
 *      b: framepointer - 0x328
 *      c: framepointer - 0x330
 * ACTUAL
 *      a: framepointer - 0x8
 *      b: framepointer - 0x10
 *      c: framepointer - 0x330
 *
 * CHANGELOG
 *      May. 2, 2012          reported and fixed by Xiang
 */

void main()
{
    long a;
    long b[100];
    long c;
    b[7] = 10;
}