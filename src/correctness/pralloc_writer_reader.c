#include "./libi0/stddef.h"
#include "./libi0/stdio.h"
#include "./libi0/malloc.h"

void reader(long **data, long data_size, long id)
{
    long i;
     
    i = 0;
main_j2:
    if(i != (*data)[i])
    {
        *(long*) STDOUT = 'E';
        *(long*) STDOUT = 'R';
        *(long*) STDOUT = 'R';
        *(long*) STDOUT = 'O';
        *(long*) STDOUT = 'R';
        *(long*) STDOUT = 10;
    }
    i = i + 100;
    if (i < data_size) {
        goto main_j2;
    }

    return;   
}

void writer(long **data, long data_size, long id)
{
    long i;

    *data = (long*)pralloc(data_size * 8);

    i = 0;
main_j1:
    (*data)[i] = i;
    i = i + 1;
    if (i < data_size) {
        goto main_j1;
    }

    return;
}

void exec(long data_size, long id)
{
    long *data;

    writer(&data, data_size, id);
    reader(&data, data_size, id);

    return;
}

void mymain()
{
    exec(0x100, 0);
    exec(0x100, 1);
    exec(0x25000, 2);
    exec(0x20000, 3);
    exec(0x1FFF0, 4);
    exec(0x100, 5);
    exec(0x100, 6);
    exec(0x12345, 7);
 //   exec(0xFFBE000, 8);

    commit;
}

void main()
{
    runner mymain();

    commit;
}
