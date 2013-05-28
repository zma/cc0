/*
 * BUG: struct support
 * 
 * DESCRIPTION
 *      The file is actually copied from:
 *      src/toolchain/test/struct.c
 * 
 * EXPECTED
 *      compile
 * 
 * ACTUAL
 *      failes to compile. 'Aborted (core dumped)' message printed out
 *      by disacc.
 * 
 * CHANGELOG
 *      Jul. 4, 2012          reported by Zhiqiang
 */

#include "libi0/stdio.h"

struct LinkNode
{
    long value;
    struct LinkNode *next;
};

void main()
{
    long three;

    struct LinkNode node1;
    struct LinkNode node2;
    struct LinkNode node3;
    struct LinkNode *p;
 
    *(long *)(0x100000208) = (long)'1';
    *(long *)(0x100000208) = (long)':';
    output_q(three);
    *(long *)(0x100000208) = (long)10;
 
    three = 0;
  
    *(long *)(0x100000208) = (long)'2';
    *(long *)(0x100000208) = (long)':';
    output_q(three);
    *(long *)(0x100000208) = (long)10;

    asm("nop");

    node1.value = 1;
    asm("nop");
    node2.value = 2;
    node3.value = 3;
 
    *(long *)(0x100000208) = (long)'3';
    *(long *)(0x100000208) = (long)':';
    output_q(three);
    *(long *)(0x100000208) = (long)10;
    
    node1.next = &node2;
    node2.next = &node3;
    node3.next = (struct LinkNode *)0;
  
    asm("nop");
    three = node3.value;
    asm("nop");

    *(long *)(0x100000208) = (long)'4';
    *(long *)(0x100000208) = (long)':';
    output_q(three);
    *(long *)(0x100000208) = (long)10;
    
    // p = &node3;
    
    // three = p->next->next->value; 
    // three = p->value;
 
    commit;
    return;
}

