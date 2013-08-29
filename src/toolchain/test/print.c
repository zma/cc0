
void output_q(long n)
{
    *(long*)0x100000208 = n;
    return;
}

void output_char(char c)
{
    *(long *)(0x100000208) = c;
    return;
}

long reducer_num;

void main()
{
    long reducer_id;
    long tmp;
    long tmp2;

    tmp2 = 1;
    reducer_num = 1;

    reducer_id = 594;

    output_q(reducer_id);

    output_q(reducer_num);

    tmp = reducer_id / reducer_num;

    output_q(tmp);

    tmp = reducer_id / tmp2;

    output_q(tmp);

    tmp = reducer_id / 1;

    output_q(tmp);

    tmp = tmp * reducer_num;

    output_q(tmp);

    reducer_id = reducer_id - tmp; // (reducer_id / reducer_num) * reducer_num;

    output_q(reducer_id);
    
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');
    output_char('a');

    commit;
}


