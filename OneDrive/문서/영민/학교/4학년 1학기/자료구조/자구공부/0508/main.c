#include <stdio.h>
#include <stdlib.h>

typedef struct _GNode
{
    int id;
    struct _GNode* next;
}GNode;

typedef struct
{
    int num;
    GNode** heads;
}Graph;

void CreateGraph(Graph* pgraph, int num);
void DestroyGraph(Graph* pgraph);
void AddEdge(Graph* pgraph, int src, int dest);
void PrintGraph(Graph* pgraph);
void DFS(Graph* pgraph);
void BFS(Graph*pgraph);

void CreateGraph(Graph* pgraph, int num)
{
    pgraph->num=num;
    pgraph->heads=(GNode **)malloc(sizeof(GNode*)* num);
    for(int i=0; i<num;i++){
        pgraph->heads[i]=(GNode *)malloc(sizeof(GNode));
        pgraph->heads[i]->next=NULL;
    }
}

void DestroyGraph(Graph* pgraph)
{
    for(int i=0;i<pgraph->num;i++){
        GNode* cur=pgraph->heads[i];
        while(cur!=NULL){
            GNode* temp=cur;
            cur=cur->next;
            free(temp);
        }
    }
    free(pgraph->heads);
}

void AddEdge(Graph* pgraph, int src, int dest)
{
    GNode* newNode1, *newNode2, *cur;

    newNode1=(GNode *)malloc(sizeof(GNode));
    newNode1->id=dest;
    newNode1->next=NULL;

    cur=pgraph->heads[src];
    while(cur->next !=NULL)
        cur=cur->next;
    cur->next=newNode1;

    newNode2=(GNode *)malloc(sizeof(GNode));
    newNode2->id=src;
    newNode2->next=NULL;

    cur=pgraph->heads[dest];
    while(cur->next !=NULL)
        cur=cur->next;
    cur->next=newNode2;
}

int main()
{
    Graph g;
    CreateGraph(&g, 5);
    AddEdge(&g, 0, 1);
    AddEdge(&g, 0, 2);
    AddEdge(&g, 0, 4);
    AddEdge(&g, 1, 2);
    AddEdge(&g, 2, 3);
    AddEdge(&g, 2, 4);
    AddEdge(&g, 3, 4);

    PrintGraph(&g);
    DestroyGraph(&g);

    return 0;
}

void DFS(Graph* pgraph)
{
    Stack stack;
    bool* visited=(bool *)malloc(sizeof(bool)* pgraph->num);
    for(int i=0;i<pgraph->num;i++)
        visited[i]=false;

    InitStack(&stack);
    Push(&stack,0);
    while(!IsSEmpty(&stack)){
        GNode* cur;
        int vtx=SPeek(&stack);
        Pop(&stack);

        if(visited[vtx]) continue;
        else{
            visited[vtx]=true;
            printf("%d",vtx);
        }
        cur=pgraph->head[vtx]->next;
        while(cur!=NULL){
            if(!visited[cur->id])
                Push(&stack,cur->id);
            cur=cur->next;
        }

    }
}

void BFS(Graph* pgraph)
{
    Queue queue;
    bool* visited=(bool *)malloc(sizeof(book)* pgraph->num);
    for(int i=0;i<pgraph->num;i++)
        visited[i]=false;

    InitQueue(&queue);
    EnQeue(&queue,0);
    while(!IsQEmpty(&queue)){
        GNode* cur;
        int vtx=QPeek(&queue);
        DeQueue(&queue);

        if(visited[vtx]) continue;
        else{
            visited[vtx]=true;
            printf("%d",vtx);
        }
        cur=pgraph->head[vtx]->next;
        while(cur!=NULL){
            if(!visited[cur->id])
                EnQueue(&queue,cur->id);
            cur=cur->next;
        }

    }
