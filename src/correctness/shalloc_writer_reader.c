#include "./libi0/stddef.h"
#include "./libi0/stdio.h"
#include "./libi0/malloc.h"

void reader(long* data, long DATA_SIZE, long id)
{
    long i;
     
    i = 0x17;
main_j2:
    if(i != data[i])
    {
        *(long*) STDOUT = 'E';
        *(long*) STDOUT = 'R';
        *(long*) STDOUT = 'R';
        *(long*) STDOUT = 'O';
        *(long*) STDOUT = 'R';
        *(long*) STDOUT = 10;
    }
    i = i + 100;
    if (i < DATA_SIZE) {
        goto main_j2;
    }
    
    commit;
}

void null_runner()
{
   commit;
}

void writer(long DATA_SIZE, long id)
{
    long i;
    long *data;

    data = (long*)shalloc(DATA_SIZE * 8);

    i = 0;
main_j1:
    data[i] = i;
    i = i + 1;
    if (i < DATA_SIZE) {
        goto main_j1;
    }

    runner null_runner();
    runner reader(data, DATA_SIZE, id)
      using data[0,,DATA_SIZE];    
    
    return;
}

void exec()
{
   writer(0x100, 0);
   writer(0x100, 1);
   writer(0x25000, 2);
   writer(0x20000, 3);
   writer(0x1FFF0, 4);
   writer(0x100, 5);
   writer(0x100, 6);
   writer(0x12345, 7);

   commit;
}

void main()
{
   runner exec();

   commit;
}
