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



struct LinkNode
{
    long value;
    struct LinkNode *next;
};

void main()
{
    long three;
    
    // struct LinkNode node1;
    // struct LinkNode node2;
    struct LinkNode node3;
    struct LinkNode *p;
    
    // node1.value = 1;
    // node2.value = 2;
    // node3.value = 3;
    
     
    // node1.next = &node2;
    // node2.next = &node3;
    // node3.next = (struct LinkNode *)0;
     
    p = &node3;
   
    // three = node3.value;

    // three = p->next->next->value; 
    // bug here
    three = p->value;
    // three = (*p).value;

    // p = p->next;

    // *(long *)(0x100000208) = three; // Should be 3;
     
    return;
}

