long v;

int main()
{
    int i;
    long l;
    unsigned int u;
    
    l = (int)(3.14); // Implicit conversion. 
    // NOTE: Constant propagation should compute (int)3.14 as 3 at compile time
    
    v = (long)9; // Explicit conversion (although not required)
    i = (int)l; // Explicit conversion
    v = i + l; // Should be equivalent to v = (long)i + l;
    i = 'c'; // Should be ok, implicit conversion;
    u = (unsigned int)i;
    
    return i;
}


