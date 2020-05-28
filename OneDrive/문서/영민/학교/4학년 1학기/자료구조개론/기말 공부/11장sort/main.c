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
        //찾은 최소값을 앞으로 빼주는 역할을 함.
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
            //인접한 아이템을 비교함.
            if(list[j]>list[j+1])
            {
                //순서가 바뀌없으면 sort해줌.
                SWAP(list[j],list[j+1],temp);
            }
        }
    }
}

//Insertion sort
void InsertionSort(Data* list,int n)
{
    int j,key;
    //i는 1부터 시작함!!
    for(int i=1;i<n;i++)
    {
        //key값이 비교할 값임.
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
