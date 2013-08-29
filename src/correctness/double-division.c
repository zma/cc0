#include "libi0/stdio.h"

long array[1000];

void init() {
    double a, b;

    long tmp;

    double *ap;

    //bug2: double division hangs program

    a = 1.0;
    b = 2.0;
    a = a/b; //comment out this line, then the program does not get stuck

    // output_q(*(long*)&a);
    // tmp = *(long*)(&a);

    // bug here
    ap = &a;

    tmp = *(long*)ap;

    output_q(tmp);

    output_char(C_n);

    commit;
}

void main(){
    runner init()
        using array[0,,1000];

    commit;
}

