#include "libi0/stdio.h"

#define DATA_SIZE 0x8000
// #define DATA_SIZE 0x8000000
// #define DATA_SIZE 0x10000000
// #define DATA_SIZE 0x1000000
#define NUM_SPACING 1000
// #define IS_OUTPUT 1
#define IS_OUTPUT 0

long* data;

void null_runner()
{
    commit;
}

void output_error()
{
    output_char((long)'E');
    output_char((long)'R');
    output_char((long)'R');
    output_char((long)'O');
    output_char((long)'R');
    output_char((long)'!');
    output_char((long)C_n);
    return;
}

long the_data(long i)
{
    return i*8;
}

void reader()
{
    long i;
    long is_output;
    long correct;

    i = 0;
    is_output = IS_OUTPUT;

    output_char((long)'R');
    output_char((long)':');
    output_char((long)C_n);
    correct = 1;
    for (i = 0; i < DATA_SIZE; i = i + NUM_SPACING) {
        // output every SPACING numbers
        if (is_output == 1) {
            output_char((long)'@');
            output_q(i);
            output_char((long)':');
            output_char((long)' ');
            output_q(data[i]);
            output_char((long)C_n);
        }
        if (data[i] != the_data(i)) {
            output_error();
            correct = 0;
            abort;
        }
    }

    if (correct != 0) {
        output_char((long)'P');
        output_char((long)'a');
        output_char((long)'s');
        output_char((long)'s');
        output_char((long)C_n);
    }

    commit;
}

void writer()
{
    long i;
    long is_output;

    i = 0;
    is_output = IS_OUTPUT;

    output_char((long)'W');
    output_char((long)':');
    output_char((long)C_n);

main_j1:
    data[i] = the_data(i);
    i = i + 1;
    if (i < DATA_SIZE) {
        goto main_j1;
    }

    // output every SPACING numbers
    if (is_output == 1) {
        i = 0;
main_j2:
        output_char((long)'@');
        output_q(i);
        output_char((long)':');
        output_char((long)' ');
        output_q(data[i]);
        output_char((long)C_n);
        i = i + NUM_SPACING;
        if (i < DATA_SIZE) {
            goto main_j2;
        }
    }

    // runner null_runner();

    runner reader()
        using data, data[0,,DATA_SIZE];

    commit;
}

void init_data(long order)
{
    // initialize data
    long i;
    long is_output;
    // start runners
    i = 0;
    is_output = 0;

    output_char((long)'I');
    output_char((long)':');
    output_char((long)C_n);

init_data_j1:
    data[i] = 0; // the_data(i);
    i = i + 1;
    if (i < DATA_SIZE) {
        goto init_data_j1;
    }

    // output every SPACING numbers
    if (is_output == 1) {
        i = 0;
init_data_j2:
        output_char((long)'@');
        output_q(i);
        output_char((long)':');
        output_char((long)' ');
        output_q(data[i]);
        output_char((long)C_n);
        i = i + NUM_SPACING;
        if (i < DATA_SIZE) {
            goto init_data_j2;
        }
    }

    if (order == 0) {
        // runner null_runner();
        runner init_data(1)
            using data, data[0,,DATA_SIZE];
    }

    commit;
}

void init(long only_reader)
{
    data = (long*)PPM_START;

    if (only_reader == 3) {
        runner init_data(0)
            using data, data[0,,DATA_SIZE];
    }

    if (only_reader == 1) {
        runner reader()
            using data, data[0,,DATA_SIZE];
        runner reader()
            using data, data[0,,DATA_SIZE];
    }

    if (only_reader == 0) {
        runner writer()
            using data, data[0,,DATA_SIZE];
    }

    commit;
}
