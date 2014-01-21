#ifndef TIME_H
#define TIME_H


#include "stdint.h"
#include "stddef.h"

#define time_t long
#define sizeof_time_t sizeof_long

/* gettime sets the amount time (in tv_sec seconds and tv_nsec
 * nanoseconds) since an unspecified point in the past (for example,
 * system start-up time, or the Epoch) on the VPC a task running on.
 *
 * Note: different VPCs possibly have different start point.
 */
long gettime(time_t *tv_sec, long *tv_nsec)
{
    long ret;

    // set system call id
    *(long*) SYSCALL_ID_ADDR = (long) SYSCALL_ID_GETTIME;

    // set system call input arguments: none here

    // call soft interrupt of system call
    asm("int 0x80");

    // get output value of system call
    ret = *(long*) ((long)SYSCALL_COMM_AREA_ADDR);
    *tv_sec = *(time_t*) ((long)SYSCALL_COMM_AREA_ADDR + sizeof_long);
    *tv_nsec = *(long*) ((long)SYSCALL_COMM_AREA_ADDR + sizeof_long + sizeof_time_t);

    return ret;
}
 
#endif // TIME_H
