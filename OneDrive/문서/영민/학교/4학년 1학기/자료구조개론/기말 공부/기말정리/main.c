#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP 100

typedef enum {false,true} bool;

typedef char Data;

typedef struct
{
    Data data;
    int priority;
}HNode;

typedef struct
{
    HNode items[MAX_HEAP + 1];
    int num;
} Heap;

void Insert(Heap *pheap,Data data,int priority)
{
    HNode newNode;
    int idx=pheap->num +1;
    if (isFull(pheap)) exit (1);

    //�ִ� ��尡 �θ𺸴� priority�� ū ���!
    while (idx>1){
        int parent=GetParent(idx);
        if(priority>pheap->items[parent].priority){
            pheap->items[idx]=pheap->items[parent];
            idx=parent;
        }
        else break;
    }
    newNode.data=data;
    newNode.priority=priority;

    pheap->items[idx]=newNode;
    pheap->num++;
}

void HeapSort(Data a[], int n)
{
    Heap heap;
    InitHeap(&heap);

    for(int i=0;i<n;i++)
        Insert(&heap, a[i],a[i]);

    for(int i=n-1; i>=0; i--)
        a[i]=Delete(&heap);
}

//counting sort
void main()

{
	int Data[MAXLEN] = {8, 1, 3, 2, 9, 9, 7, 10, 5, 2}; // item is 1 <= N <= 10
	int ret[MAXLEN] = { 0, };
	int count[MAXLEN];
	int idx = 0;

	//init count index
	for (int i = 0; i <= MAXLEN; i++)
		count[i] = 0;

	for (int i = 0; i < MAXLEN; i++)
	{
		count[Data[i]] += 1;
	}

	idx = MAXLEN - 1;

	for (int i = MAXLEN; i > 0; i--)

	{
		while (count[i] > 0)
		{
			ret[idx--] = i;
			count[i] -= 1;
		}
	}
	printf("{");

	for (int i = 0; i < MAXLEN; i++)

	{
		printf("%d,", ret[i]);
	}

	printf("}\n");


//insertion sort

void InsertionSort(Data* list, int n)
{
    int j,key;

    for(int i=1; i<n; i++)
    {
        key=list[i];

        for(j=i-1; j>=0; j--){
            if(key<list[j])
                list[j+1]=list[j];
            else
                break;
        }
        list[j+1]=key;
    }
}

//Quick sort
int Partition(Data* list,int left, int right)
{
    int pivot=list[left],temp;
    int low=left+1,high=right;

    while(1){
        while(low<right && list[low]<pivot)
            low++;
        while(high>left && list[high]>=pivot)
            high--;

        if(low<high)
            SWAP(list[low],list[high],temp);
        else break;

    }
    SWAP(list[left],list[high],temp);
    return high;
}

int Partition(Data* list,int left, int right)
{
     int pivot=list[left],temp;
     int low=left+1, high=rigt;

     while(1){
        while(low<right && list[low]<pivot)
            low++;
        while(high>left && list[high]>=piot)
            high--;
        if(low<high)
            SWAP(list[low],list[high],temp);
     }
     SWAP(list[left],list[high],temp);
     return high;
}

void QuickSort(Data *list, int left, int right)
{
    if(left<right)
    {
        int mid=Partition(list,left,right);
        QuickSort(list,left,mid-1);
        QuickSort(list,mid+1,right);
    }
}

void QuickSort(Data* list, int left, int right)
{
    if(left<right)
    {
        int mid=Partition(list,left,right);
        QuickSort(list,left,mid-1);
        QuickSort(list,mid+1,right);
    }
}

//merge sort
void Merge(Data* list, int left, int mid, int right)
{
    int sorted[MAX_SIZE];
    //�ΰ��� ��Ȱ�Ȱ���.
    int first=left,second=mid+1,i=left;

    while(fist<=mid && second <=right){
        //first ��  �� ������ first ���� �־���.
        if(list[first]<=list[second])
            sorted[i++]=list[first++];
        //second �� �� ������ second ���� �־���.
        else
            sorted[i++]=list[second++];
    }

    //ù������ �ι������� ũ
    if(first>mid)
        //second ���� �־���.
        for(int j=second; j<=right;j++)
            sorted[i++]=list[j];
    else
        //first ���� �־���.
        for(int j=first; j<=mid;j++)
            sorted[i++]=list[j];

    //���ĵ� ������ list�� �־���.
    for(int j=left;j<=right;j++)
        list[j]=sorted[j];

}

void MergeSort(Data* list, int left, int right)
{
    if(left<right)
    {
        int mid=(left+right)/2;
        //ó�� ���� �߰���
        MergeSort(list,left,mid);��
        //�߰����� ��������
        MergeSort(list,mid+1,right);
        //���� �ΰ��� ��ģ��.
        Merge(list,left,mid,right);
    }
}

void Merge(Data* list, int left, int mid, int right)
{
    int sorted[MAX_SIZE];
    //divide into two first
    int first=left, second=mid+1, i=left;

    //�ϴ� �ѷ� ���� first,second �־���.
    while(first<=mid && second<=right){
        if(list[first]<=list[second])
            sorted[i++]=list[first++];
        else
            sorted[i++]=list[second++];
    }

    if(first>mid)
        for(int j=second;j<=right;j++)
            sorted[i++]=list[j];
    else
        for(int j=first;j<=mid;j++)
            sorted[i++]=list[j];

    for(int j=left;j<=rigt;j++)
        list[j]=sorted[j];
}

