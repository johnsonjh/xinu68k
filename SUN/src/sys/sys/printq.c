#include <q.h>

printq(head)
int head;
{
    int i, tail;

    tail = head + 1;
    printf("queue contents:\n\n");
    for(i=q[head].qnext; i != tail; i=q[i].qnext)
	kprintf("key: %d next: %d prev: %d\n",q[i].qkey,q[i].qnext,q[i].qprev);
}
