long a[100];

void main()
{
    long i;
    void *p;
    long *ip;
    
    p = (void*)123456; // Should be OK;
    
    ip = (long*)p;
    
    i = 9;
    ip = &i;
    
    a[0] = *ip;  // ip points to 123456 which is in the PR range
    
   
    ip = a;
    *ip = 10;
    
    ip = &a[i];
    *ip = ip[1];
    
    return;
}
