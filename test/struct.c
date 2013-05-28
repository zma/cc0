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
    
    node1.value = 1;
    node2.value = 2;
    node3.value = 3;
    
     
    node1.next = &node2;
    node2.next = &node3;
    node3.next = (struct LinkNode *)0;
    
    p = &node3;
    
    three = p->next->next->value; 
    three = p->value;

     *(long *)(0x100000208) = three; // Should be 3;
     
    return;
}
