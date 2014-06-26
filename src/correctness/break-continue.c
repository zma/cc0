#include <stdio.h>

void runner1()
{
    long i;

    output_char('A');
    output_char(C_n);

    for (i = 0; i < 10; i = i + 1) {
        if (i == 8) {
            break;
        }
        output_q(i);
        output_char(C_n);
    }

    output_char('B');
    output_char(C_n);

    for (i = 0; i < 10; i = i + 1) {
        if (i == 8) {
            // continue;
            goto GOTO_RUNNER1_1;
        }
        output_q(i);
        output_char(C_n);
GOTO_RUNNER1_1:
    }

    output_char('O');
    output_char('K');
    output_char(C_n);
    return;
}

void main()
{
    runner1();

    commit;
}


