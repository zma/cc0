/*
 * BUG: char to long cast
 * 
 * DESCRIPTION
 *      char to long cast seems not correct
 * 
 * EXPECTED
 *   the output_char moves c from char (sb) to long (sq)   
 * 
 * ACTUAL
 *   moved from char (sb) to char (sb)
 *   40000010E>  mov $0x10(0x200000000):sb, 0x100000208:sb
 *
 * 
 * CHANGELOG
 *      April 21, 2012          reported by Zhiqiang
 *      April 22, 2012          fixed by Xiang. NOTE: Please use explict conversion, implicity conversion may not work currently.
 *      April 24, 2012          reopened by Zhiqiang. (long)'a' does not
 *      work while (long)20 works. Possibly bug from compiler or
 *      translator. need to further checking.
 *      April 24, 2012          fixed by Zhiqiang. -- the previous
 *      report one is caused by the bin translator.
 *      April 25, 2012          verified by Xiang, 
 */

void output_char(char c)
{
    *(char *)(0x100000208) = (char)c;
    return;
}

void main()
{
    // okay
    output_char((char)20);
    
    // Not okey, explicit conversion is required
    //output_char(22);

    // not okay
    output_char('a');

    commit;
}

