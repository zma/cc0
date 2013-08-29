#include "libi0/stdio.h"

#define no_DEBUG_

void runner1()
{
    char buf[100];
    long q;
    long ok;

#ifdef _DEBUG_
    output_char('b');
    output_char(C_n);
#endif

    q = input_q();

#ifdef _DEBUG_
    output_q(q);
    output_char(C_n);
#endif

    if (q != 29889) {
        output_char('N');
        output_char(C_n);
        abort;
    }

    q = input_q();

#ifdef _DEBUG_
    output_q(q);
    output_char(C_n);
#endif

    if (q != -29889) {
        output_char('N');
        output_char(C_n);
        abort;
    }

    q = input_q();

#ifdef _DEBUG_
    output_q(q);
    output_char(C_n);
#endif

    if (q != 0) {
        output_char('N');
        output_char(C_n);
        abort;
    }

    q = input_q();

#ifdef _DEBUG_
    output_q(q);
    output_char(C_n);
#endif

    if (q != 88) {
        output_char('N');
        abort;
    }

    output_char('O');
    output_char('K');
    output_char(C_n);

    commit;
}

void main()
{
    runner1();

    commit;
}


