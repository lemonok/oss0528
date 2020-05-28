#include <stdio.h>
#include <stdlib.h>��

//������ ���� heap ����ü ����

typedef Heap PQueue;

void InitPQueue(PQueue* ppqueue)
{
    InitHeap(ppqueue);
}

bool IsPQEmpty(PQueue* ppqueue)
{
    return IsEmpty(ppqueue);
}

bool IsPQFull(PQueue* ppqueue) {
	return IsFull(ppqueue);
}

void Enqueue(PQueue * ppqueue, Data data, int priority) {
	Insert(ppqueue, data, priority);
}

Data Dequeue(PQueue * ppqueue) {
	return Delete(ppqueue);
}
