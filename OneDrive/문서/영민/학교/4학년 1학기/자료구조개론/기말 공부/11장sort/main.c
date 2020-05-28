#include <stdio.h>
#include <stdlib.h>

//selective sort

void SelectionSort(Data* list,int n)
{
    int min,temp;
    for(int i=0;i<n-1;i++)
    {
        min=i;
        for(int j=i+1;j<n;j++)
        {
            if(list[j]<list[min])
                min=j;
        }
        //ã�� �ּҰ��� ������ ���ִ� ������ ��.
        SWAP(list[i],list[min],temp)
    }
}

//Bubble sort

void BubbleSort(Data* list,int n)
{
    int temp;
    for(int i=n-1;i>0;i--)
    {
        for(int j=0;j<i;j++)
        {
            //������ �������� ����.
            if(list[j]>list[j+1])
            {
                //������ �ٲ������ sort����.
                SWAP(list[j],list[j+1],temp);
            }
        }
    }
}

//Insertion sort
void InsertionSort(Data* list,int n)
{
    int j,key;
    //i�� 1���� ������!!
    for(int i=1;i<n;i++)
    {
        //key���� ���� ����.
        key=list[i];
        //
        for(j=i-1;j>=0;j--){
            if(key<list[j])
                list[j+1]=list[j];
            else
                break;
        }
        list[j+1]=key;
    }
}
