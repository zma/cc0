
void fun_call(long l);

void main()
{

    fun_call(20000);
    fun_call(10000);
    fun_call(30000);

    commit;
}


void fun_call(long l)
{
    *(long *)(0x100000208) = l;

    return;
}
