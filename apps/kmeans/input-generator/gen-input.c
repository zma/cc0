// Linux program.
// Generate input for k-means

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

#define SIZE_INT 32

// number of point
#define NUM_DIM 4

long rand_long() 
{
    int lower_32bit, upper_32bit;   
    long rand_long;

    /* generate random number: */
    lower_32bit = rand();
    upper_32bit = rand();
    return (((long)upper_32bit << SIZE_INT) + lower_32bit);
}

int main(int argc, char* argv[]) 
{
    long i, j, ran_num;

    if (argc < 2) {
        printf("Usage: %s num_points\n", argv[0]);
        return 0;
    }

    long num_point = atoi(argv[1]);

    FILE* f1 = NULL;

    f1 = fopen("kmeans-input.301", "w");

    /* initialize random seed: */
    srand ( 0 );

    for (i = 0; i < num_point; i++)
    {
        for (j = 0; j < NUM_DIM; j++)
        {
             ran_num = rand_long();
             ran_num = ran_num / 16L;
             // Note: here, printing a long as a %d is a trick:
             // cast the lower 32bit of the long as an 32bit integer
             fprintf(f1, "%d ", ran_num);
        }
        fprintf(f1, "\n");
    }
    
}
