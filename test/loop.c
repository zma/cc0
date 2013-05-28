void main()
{
    long a;
    long i;
    
    for (i = 0; i < 100; i = i + 1)
    {
        a = a + i;
    }
    
    while (a < 0)
    {
        i = i + 1;
        a = a + 1;
    }
    
    do 
    {
        a = a * 2;
        i = i - 1;
    } while (i > 0);
    
    commit;
}