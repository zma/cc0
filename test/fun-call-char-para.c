/*
 * 
 * DESCRIPTION
 * 
 * EXPECTED
 * 
 * ACTUAL
 * 
 * CHANGELOG
 */

void output_char(char c)
{
    *(long *)(0x100000208) = (long)c;
    return;
}

void main()
{
    output_char((char)20);
    output_char((char)22);

    output_char((char)'a');

    commit;
}

