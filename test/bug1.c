void main()
{
    long bb, ii, jj, tmp, shift_num, my_key;
    long start, end;
    
    start = 0,end = 1;
    bb = 1047552;
    shift_num = 10;

    ii = start;
gen_histogram_j3:
    if(ii < end)
    {
        tmp = 1024;
        my_key = tmp & bb;
       
        *(long *)(0x100000208) = my_key;
        // my_key = my_key >> shift_num;
    
        jj = 0;
    gen_histogram_j4:
        if(jj < shift_num)
        {
            if(my_key == 0)
            {
                goto gen_histogram_j4_out;
            }
            else
            {
                my_key = my_key / 2;
                jj = jj + 1;
                *(long *)(0x100000208) = jj;
                *(long *)(0x100000208) = my_key;
                goto gen_histogram_j4;
            }
        }
gen_histogram_j4_out:
        *(long *)(0x100000208) = 125;
        *(long *)(0x100000208) = my_key;
        *(long *)(0x100000208) = 125;

        ii = ii + 1;
        goto gen_histogram_j3;
    }
    
    commit;
}
